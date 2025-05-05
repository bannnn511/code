
#include "mapreduce.h"
#include "shard.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdbool.h>
#include "kv.h"

// int shard_size = 16 * 1024; // 16kb

#define PATH_MAX 256

typedef unsigned long ul;
Partitioner partitioner;
ul num_partitions = 1;
char *intermediate_file = "./tasktracker";

void map_worker(shard s, Mapper map);

void reduce_worker(Reducer reducer, ul partition_number);

void notify_partition_complete(ul partition_number);

void wait_for_partition(ul partition_number);

void init_partition_status(ul num_partitions);


typedef struct {
    char **keys;
    char **values;
    int size;
    int capacity;
    time_t last_flush;
} KeyValueBuffer;

KeyValueBuffer *buffers = NULL;
int buffer_capacity = 1000;
int flush_threshold = 100; // when to flush to disk
int flush_interval_sec = 10; // flush every 10 seconds


// init_buffers run after files are sharded
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

    ul len = strlen(intermediate_file) + strlen("/intermidiate_") + 10;

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


    char *filename;
    create_intermediate_dir(&filename, partition_number);
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < buffer->size; i++) {
        fprintf(fp, "%s\t%s\n", buffer->keys[i], buffer->values[i]);
        printf("write to partition %ld", partition_number);
        free(buffer->keys[i]);
        free(buffer->values[i]);
    }

    fflush(fp);
    buffer->last_flush = time(NULL);
    buffer->size = 0;
}


void check_timed_flushed() {
    if (buffers == NULL) {
        return;
    }

    const time_t current_time = time(NULL);
    for (ul i = 0; i < num_partitions; i++) {
        const KeyValueBuffer buffer = buffers[i];
        if (buffer.size < 0) {
            continue;
        }

        if (difftime(current_time, buffer.last_flush) > 0) {
            flush_buffer(i);
        }
    }
}

// append_buffer flushes buffer if exceed time threshhold or capacity
void append_buffer(const ul partition_number, char *key, char *value) {
    if (buffers == NULL || partition_number >= num_partitions) {
        fprintf(stderr, "[ERROR][mapreduce.c] append_buffer: invalid partition number %ld\n", partition_number);
        return;
    }

    check_timed_flushed();

    KeyValueBuffer *buffer = &buffers[partition_number];
    if (buffer->size == buffer->capacity) {
        flush_buffer(partition_number);
    }

    buffer->keys[buffer->size] = strdup(key);
    buffer->values[buffer->size] = strdup(value);
    buffer->size++;
}


char *get_next(char *key, int partition_number) {
    // const ul partition_no = partitioner(key, num_partitions);
    char *value = "a";

    return value;
}

// MR_Emit should not flush buffer
void MR_Emit(char *key, char *value) {
    const ul partition_no = partitioner(key, num_partitions);

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

    shard_file(s, files, argc - 1);
    num_partitions = num_reducers;
    init_buffers(num_partitions);
    init_partition_status(num_partitions);

    for (int i = 0; i < s->size; i++) {
        map_worker(s->arr[i], map);
    }

    for (ul i = 0; i < num_partitions; i++) {
        flush_buffer(i);
        notify_partition_complete(i);
    }

    // 2. start worker pools
    // thread_pool *pool = malloc(sizeof(thread_pool));
    // if (pool == NULL) {
    //     perror("malloc");
    //     exit(EXIT_FAILURE);
    // }

    // Start reducers when partitions are ready
    for (int i = 0; i < num_reducers; i++) {
        wait_for_partition(i);
        // reduce(NULL, get_next, i);
        reduce_worker(reduce, i);
    }

    printf("done\n");

    free(files);
    free(s);
    // free(pool);
}

typedef struct partition_status {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int completed_mappers;
    int total_mappers;
    bool is_ready;
    bool has_data;
} partition_status;

// Add to global variables
partition_status *partition_statuses = NULL;

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

void map_worker(const shard s, const Mapper map) {
    const size_t buffer_size = s.end - s.start + 1;
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    if (read_shard(s, buffer) == -1) {
        fprintf(stderr, "readshard");
        exit(EXIT_FAILURE);
    }

    map(buffer);
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
        char *value = strtok(NULL, "\t");
        if (!key || !value) continue;

        kv_append(kv, key, value);
    }
}

void reduce_worker(const Reducer reduce, const ul partition_number) {
    KeyValueStore *kvs = malloc(sizeof(KeyValueStore));
    if (kvs == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    init_kvs(kvs);
    process_key_values(kvs, partition_number);
    free_kvs(kvs);
}
