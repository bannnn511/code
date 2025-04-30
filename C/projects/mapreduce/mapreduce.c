#include "mapreduce.h"
#include "shard.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

// int shard_size = 16 * 1024; // 16kb

#define PATH_MAX 256

Partitioner partitioner;
int num_partitions = 1;
char *intermediate_file = "./tasktracker";

void map_worker(shard s, Mapper umap_func);

typedef struct {
    char **keys;
    char **values;
    int size;
    int capacity;
    time_t last_flush;
} KeyValueBuffer;

KeyValueBuffer *buffers = NULL;
int buffer_capacity = 2;
int flush_threshold = 100; // when to flush to disk
int flush_interval_sec = 1; // flush every 10 seconds

// init_buffers run after files are sharded
void init_buffers(int num_partitions) {
    if (buffers != NULL) {
        return;
    }

    buffers = calloc(num_partitions, sizeof(KeyValueBuffer));
    if (buffers == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    time_t current_time = time(NULL);
    for (int i = 0; i < num_partitions; i++) {
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

void create_intermediate_dir(char **filename_ptr, int partition_number) {
    struct stat st;
    if (stat(intermediate_file, &st) == -1) {
        if (mkdir(intermediate_file, 0755) == -1) {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }

    int len = strlen(intermediate_file) + strlen("/intermidiate_") + 10;

    char *filename = malloc(len);
    if (filename == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    if (snprintf(filename, len, "%s/intermediate_%d.txt",
                 intermediate_file, partition_number) >= len) {
        free(filename);
        fprintf(stderr, "Path too long\n");
        exit(EXIT_FAILURE);
    }

    *filename_ptr = filename;
}

void flush_buffer(int partition_number) {
    printf("[INFO][mapreduce.c] flush_buffer partition %d\n", partition_number);
    KeyValueBuffer *buffer = &buffers[partition_number];
    if (buffer->size == 0) {
        return;
    }


    char *filename;
    create_intermediate_dir(&filename, partition_number);
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        // fp = fopen(filename, "w");
        // if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
        // }
    }

    for (int i = 0; i < buffer->size; i++) {
        fprintf(fp, "%s\t%s\t", buffer->keys[i], buffer->values[i]);
        free(buffer->keys[i]);
        free(buffer->values[i]);
    }

    buffer->last_flush = time(NULL);
    buffer->size = 0;
}

void append_buffer(int partition_number, char *key, char *value) {
    if (buffers == NULL || partition_number < 0 || partition_number >= num_partitions) {
        fprintf(stderr, "[ERROR][mapreduce.c] append_buffer: invalid partition number %d\n", partition_number);
        return;
    }

    KeyValueBuffer *buffer = &buffers[partition_number];
    printf("[INFO][mapreduce.c] append_buffer partition %d\n", partition_number);
    if (buffer->size == buffer->capacity) {
        flush_buffer(partition_number);
    }

    printf("[INFO][mapreduce.c] append_buffer key %s value %s\n", key, value);
    buffer->keys[buffer->size] = strdup(key);
    buffer->values[buffer->size] = strdup(value);
    buffer->size++;
}

void check_timed_flushed() {
    if (buffers == NULL) {
        return;
    }

    time_t current_time = time(NULL);
    for (int i = 0; i < num_partitions; i++) {
        KeyValueBuffer buffer = buffers[i];
        if (buffer.size < 0) {
            continue;
        }

        if (difftime(current_time, buffer.last_flush) > 0) {
            printf("[INFO][mapreduce.c] timed flush partition %d\n", i);
            flush_buffer(i);
        }
    }
}

char *get_next(char *key, int partition_number) {
    char *value = "a";

    return value;
}

void MR_Emit(char *key, char *value) {
    printf("[DEBUG][MR_EMIT] emit %s %s\n", key, value);
    int partition_no = partitioner(key, num_partitions);

    check_timed_flushed();

    append_buffer(partition_no, key, value);

    // flush remaining in buffer
    flush_buffer(partition_no);
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    printf("[INFO][Partition]\n");
    unsigned long hash = 5381;
    unsigned char c;
    while ((c = *key++) != '\0') {
        hash = hash * 33 + c;
    }

    return hash % num_partitions;
}

/**
 * 1. split the input files into M pieces
 * 2. start workers, master give shards to workers
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
    num_partitions = s->size;
    init_buffers(num_partitions);
    printf("shard_file done\n");

    for (int i = 0; i < s->size; i++) {
        map_worker(s->arr[i], map);
    }

    // 2. start worker pools
    // thread_pool *pool = malloc(sizeof(thread_pool));
    // if (pool == NULL) {
    //     perror("malloc");
    //     exit(EXIT_FAILURE);
    // }

    printf("done\n");

    free(files);
    free(s);
    // free(pool);
}

typedef struct key_value {
    char *key;
    char *value;
} key_value;

typedef struct map_task {
    shard s;
    key_value *paris; // key-value pair buffer in memory
} map_task;

void map_worker(shard s, Mapper map) {
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
