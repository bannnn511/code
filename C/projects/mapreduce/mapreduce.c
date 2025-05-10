
#include "mapreduce.h"
#include "shard.h"
#include <stdatomic.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "kv.h"
#include "threadpool.h"

// int shard_size = 16 * 1024; // 16kb

#define MAX_BUFFER_SIZE 1024

typedef unsigned long ul;
Partitioner partitioner;
ul num_partitions = 1;
char *intermediate_file = "./tasktracker";

typedef struct {
    shard s;
    Mapper map;
} map_worker_task;

void map_worker(void *arg);

typedef struct {
    Reducer reduce;
    Getter get_next;
    ul partition_number;
} reduce_worker_task;

void reduce_worker(void *arg);

void notify_partition_complete(ul partition_number);

void wait_for_partition(ul partition_number);

void init_partition_status(ul num_partitions);
void create_intermediate_dir(char **filename_ptr, const ul partition_number);

typedef struct {
    char **keys;
    char **values;
    int size;
    int capacity;
    time_t last_flush;
    pthread_mutex_t mutex;
    FILE *fp;  // intermediate file pointer
} KeyValueBuffer;

typedef struct partition_status {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int completed_mappers;
    int total_mappers;
    bool is_ready;
    bool has_data;
} partition_status;

void free_partition_status(partition_status *status);

KeyValueBuffer *buffers = NULL;
int buffer_capacity = 1000;
int flush_threshold = 100;             // when to flush to disk
int flush_interval_sec = 10;           // flush every 10 seconds
size_t write_buffer_size = 64 * 1024;  // 64KB write buffer
KeyValueStore *store = NULL;
partition_status *partition_statuses = NULL;

void init_buffers(const ul num_partitions) {
    if (buffers != NULL) {
        return;
    }

    buffers = calloc(num_partitions, sizeof(KeyValueBuffer));
    if (buffers == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    time_t current_time = time(NULL);
    for (ul i = 0; i < num_partitions; i++) {
        buffers[i].keys = malloc(buffer_capacity * sizeof(char *));
        buffers[i].values = malloc(buffer_capacity * sizeof(char *));
        if (buffers[i].keys == NULL || buffers[i].values == NULL) {
            perror("[ERROR][mapreduce.c][init_buffers] malloc");
            exit(EXIT_FAILURE);
        }

        char *filename;
        create_intermediate_dir(&filename, i);
        FILE *fp = fopen(filename, "a");
        if (fp == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_init(&buffers[i].mutex, NULL);

        buffers[i].fp = fp;
        buffers[i].size = 0;
        buffers[i].capacity = buffer_capacity;
        buffers[i].last_flush = current_time;
    }
}

void create_intermediate_dir(char **filename_ptr, const ul partition_number) {
    struct stat st;
    if (stat(intermediate_file, &st) == -1) {
        if (mkdir(intermediate_file, 0755) == -1) {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }

    ul len = strlen(intermediate_file) + strlen("/intermediate_") + 10;

    char *filename = malloc(len);
    if (filename == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    snprintf(filename, len, "%s/intermediate_%ld.txt", intermediate_file, partition_number);
    *filename_ptr = filename;
}

void flush_buffer(const ul partition_number) {
    KeyValueBuffer *buffer = &buffers[partition_number];
    if (buffer->size == 0) {
        return;
    }

    char file_buf[write_buffer_size];
    size_t buffer_pos = 0;

    for (int i = 0; i < buffer->size; i++) {
        int written = snprintf(file_buf + buffer_pos, write_buffer_size - buffer_pos, "%s\t%s\n",
                               buffer->keys[i], buffer->values[i]);

        if (written > 0 && written < write_buffer_size - buffer_pos) {
            buffer_pos += written;
        } else {
            // Buffer is full, flush it
            fwrite(buffer, 1, buffer_pos, buffer->fp);
            buffer_pos = 0;

            // Write the current entry
            written = snprintf(file_buf, write_buffer_size, "%s\t%s\n", buffer->keys[i],
                               buffer->values[i]);
            if (written > 0) {
                buffer_pos = written;
            }
        }

        free(buffer->keys[i]);
        free(buffer->values[i]);
    }

    // Flush any remaining data
    if (buffer_pos > 0) {
        fwrite(file_buf, 1, buffer_pos, buffer->fp);
    }

    fflush(buffer->fp);
    buffer->last_flush = time(NULL);
    buffer->size = 0;
}

// append_buffer flushes buffer if exceed time threshhold or capacity
void append_buffer(const ul partition_number, char *key, char *value) {
    if (buffers == NULL || partition_number >= num_partitions) {
        fprintf(stderr, "[ERROR][mapreduce.c] append_buffer: invalid partition number %ld\n",
                partition_number);
        return;
    }

    KeyValueBuffer *buffer = &buffers[partition_number];
    pthread_mutex_lock(&buffer->mutex);

    // Check if we need to flush due to capacity
    if (buffer->size == buffer->capacity) {
        flush_buffer(partition_number);
    }

    // Check if we need to flush due to time
    if (buffer->size > 0) {
        time_t current_time = time(NULL);
        if (difftime(current_time, buffer->last_flush) > flush_interval_sec) {
            flush_buffer(partition_number);
        }
    }

    char *new_key = strdup(key);
    char *new_value = strdup(value);

    if (new_key != NULL && new_value != NULL) {
        buffer->keys[buffer->size] = new_key;
        buffer->values[buffer->size] = new_value;
        buffer->size++;
    } else {
        free(new_key);
        free(new_value);
    }

    pthread_mutex_unlock(&buffer->mutex);
}

void cleanup_buffers() {
    if (buffers == NULL) {
        return;
    }

    for (ul i = 0; i < num_partitions; i++) {
        pthread_mutex_lock(&buffers[i].mutex);
        flush_buffer(i);

        free(buffers[i].keys);
        free(buffers[i].values);
        pthread_mutex_unlock(&buffers[i].mutex);
        pthread_mutex_destroy(&buffers[i].mutex);
    }
    free(buffers);
    buffers = NULL;
}

char *get_next(char *key, int partition_number) {
    KeyValueStore *kv = &store[partition_number];
    if (kv == NULL) {
        fprintf(stderr, "[ERROR][mapreduce.c] get_next: invalid partition number %d\n",
                partition_number);
        return NULL;
    }

    return kv_pop(kv, key);
}

void MR_Emit(char *key, char *value) {
    const ul partition_no = partitioner(key, num_partitions);
    // printf("%s %s\n", key, value);

    append_buffer(partition_no, key, value);
}

ul MR_DefaultHashPartition(char *key, const int num_partitions) {
    ul hash = 5381;
    unsigned char c;
    while ((c = *key++) != '\0') {
        hash = hash * 33 + c;
    }

    return hash % num_partitions;
}

/**
 * 1. split the input files into M pieces
 * 2. start workers, master gives shards to workers
 */
void MR_Run(int argc, char *argv[], Mapper map, int num_mappers, Reducer reduce, int num_reducers,
            Partitioner partition) {
    if (argc < 2) {
        printf("Usage: ./mapreduce <input file> <input file> ...\n");
        return;
    }

    partitioner = partition;

    // 1. split input files into shards
    char **files = malloc(argc * sizeof(char *));
    for (int i = 0; i < argc; i++) {
        files[i] = argv[i + 1];
    }
    vector *s = malloc(sizeof(vector));
    if (s == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    init_vector(s);

    printf("[INFO][mapredeuce.c] SHARD RUNNING\n");
    shard_file(s, files, argc - 1);
    printf("[INFO][mapredeuce.c] SHARD DONE\n");

    num_partitions = num_reducers;
    init_buffers(num_partitions);
    init_partition_status(num_partitions);
    store = calloc(num_partitions, sizeof(KeyValueStore));
    if (store == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    for (ul i = 0; i < num_partitions; i++) {
        init_kvs(&store[i]);
    }

    thread_pool *mapper_pool = malloc(sizeof(thread_pool));
    if (mapper_pool == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    thread_pool *reducer_pool = malloc(sizeof(thread_pool));
    if (reducer_pool == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    thread_pool_init(mapper_pool, num_mappers, 10, 0);
    thread_pool_init(reducer_pool, num_reducers, 10, 0);

    printf("[INFO][mapredeuce.c] MAP START\n");
    for (int i = 0; i < s->size; i++) {
        map_worker_task *map_t = malloc(sizeof(map_worker_task));
        *map_t = (map_worker_task){s->arr[i], map};
        thread_pool_add(mapper_pool, map_worker, map_t);
        // map_worker(map_t); // for single worker
    }

    thread_pool_wait(mapper_pool);
    printf("[INFO][mapredeuce.c] MAP DONE\n");

    for (ul i = 0; i < num_partitions; i++) {
        flush_buffer(i);
        notify_partition_complete(i);
    }

    printf("[INFO][mapredeuce.c] REDUCE START\n");
    for (int i = 0; i < num_reducers; i++) {
        wait_for_partition(i);
        reduce_worker_task *reduce_t = malloc(sizeof(reduce_worker_task));
        *reduce_t = (reduce_worker_task){reduce, get_next, i};
        thread_pool_add(reducer_pool, reduce_worker, reduce_t);
        // reduce_worker(reduce_t);  // for single worker
    }

    thread_pool_wait(reducer_pool);
    printf("[INFO][mapredeuce.c] REDUCE DONE\n");

    // free resources
    thread_pool_destroy(mapper_pool);
    thread_pool_destroy(reducer_pool);

    // Ensure all partitions are done and files are flushed
    for (ul i = 0; i < num_partitions; i++) {
        pthread_mutex_lock(&buffers[i].mutex);
        flush_buffer(i);
        pthread_mutex_unlock(&buffers[i].mutex);
    }

    for (ul i = 0; i < num_partitions; i++) {
        free_kvs(&store[i]);
    }

    // Clean up intermediate files
    for (ul i = 0; i < num_partitions; i++) {
        char filename[MAX_BUFFER_SIZE];
        snprintf(filename, MAX_BUFFER_SIZE, "%s/intermediate_%lu.txt", intermediate_file, i);
        if (unlink(filename) != 0 && errno != ENOENT) {
            fprintf(stderr, "Failed to remove intermediate file %s: %s\n", filename,
                    strerror(errno));
        }
    }
    if (rmdir(intermediate_file) != 0 && errno != ENOENT) {
        fprintf(stderr, "Failed to remove intermediate directory %s: %s\n", intermediate_file,
                strerror(errno));
    }

    cleanup_buffers();
    free_partition_status(partition_statuses);
    free(partition_statuses);
    free_shard(s);
    free(files);
    free(s);
    free(mapper_pool);
    free(reducer_pool);
    free(store);
}

// Initialize the status tracking system
void init_partition_status(const ul num_partitions) {
    partition_statuses = calloc(num_partitions, sizeof(partition_status));
    if (!partition_statuses) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    for (ul i = 0; i < num_partitions; i++) {
        if (pthread_mutex_init(&partition_statuses[i].mutex, NULL) != 0 ||
            pthread_cond_init(&partition_statuses[i].cond, NULL) != 0) {
            perror("pthread_mutex/cond_init");
            exit(EXIT_FAILURE);
        }
        partition_statuses[i].is_ready = false;
    }
}

void free_partition_status(partition_status *status) {
    if (status == NULL) {
        return;
    }

    pthread_cond_destroy(&status->cond);
    pthread_mutex_destroy(&status->mutex);
}

void wait_for_partition(const ul partition_number) {
    pthread_mutex_lock(&partition_statuses[partition_number].mutex);
    while (!partition_statuses[partition_number].is_ready) {
        pthread_cond_wait(&partition_statuses[partition_number].cond,
                          &partition_statuses[partition_number].mutex);
    }
    pthread_mutex_unlock(&partition_statuses[partition_number].mutex);
}

void notify_partition_complete(const ul partition_number) {
    pthread_mutex_lock(&partition_statuses[partition_number].mutex);

    partition_statuses[partition_number].is_ready = true;

    pthread_cond_broadcast(&partition_statuses[partition_number].cond);
    pthread_mutex_unlock(&partition_statuses[partition_number].mutex);
}

void map_worker(void *arg) {
    const map_worker_task t = *(map_worker_task *)arg;
    const size_t buffer_size = t.s.end - t.s.start + 1;
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    if (read_shard(t.s, buffer) == -1) {
        fprintf(stderr, "readshard");
        exit(EXIT_FAILURE);
    }

    t.map(buffer);
    free(buffer);
}

void process_key_values(KeyValueStore *kv, const ul partition_number) {
    ul len = strlen(intermediate_file) + strlen("/intermediate_") + 10;
    char *filename = malloc(len);
    if (!filename) {
        perror("malloc");
        return;
    }
    snprintf(filename, len, "%s/intermediate_%ld.txt", intermediate_file, partition_number);

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        free(filename);
        return;
    }

    char *line = NULL;
    size_t size = 0;

    while (getline(&line, &size, fp) != -1) {
        char *key = strtok(line, "\t");
        char *value = strtok(NULL, "\t\n");  // Also remove newline
        if (!key || !value) continue;

        // Make the strings null-terminated at the right place
        char *nl_key = strchr(key, '\n');
        if (nl_key) *nl_key = '\0';
        char *nl_value = strchr(value, '\n');
        if (nl_value) *nl_value = '\0';

        // Skip empty strings
        if (strlen(key) == 0 || strlen(value) == 0) continue;

        kv_append(kv, key, value);
    }

    free(line);
    fclose(fp);
    free(filename);
    pthread_mutex_unlock(&kv->mu);
}

void reduce_worker(void *arg) {
    reduce_worker_task *t = arg;
    KeyValueStore *kvs = &store[t->partition_number];
    if (kvs == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    process_key_values(kvs, t->partition_number);
    kv_sort(kvs);

    for (ul i = 0; i < kvs->kv_count; i++) {
        t->reduce(kvs->kvs[i].key, get_next, t->partition_number);
    }

    free(t);
}
