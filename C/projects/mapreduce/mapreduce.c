#include "mapreduce.h"
#include "shard.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

// int shard_size = 16 * 1024; // 16kb

Partitioner partitioner;
int num_partitions = 1;
char *intermediate_file = "/local/tasktracker/";

void map_worker(shard s, Mapper umap_func);

typedef struct {
    char **keys;
    char **values;
    int size;
    int capacity;
    time_t last_flush;
} KeyValueBuffer;

KeyValueBuffer *buffers = NULL;
int buffer_capacity = 50;
int flush_threshold = 100;    // when to flush to disk
int flush_interval_sec = 10;  // flush every 10 seconds

void init_buffers() {
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
        buffers[i].size = 0;
        buffers[i].capacity = buffer_capacity;
        buffers[i].last_flush = current_time;
    }
}

void flush_buffer(int partition_number) {
    KeyValueBuffer buffer = buffers[partition_number];
    if (buffer.size == 0) {
        return;
    }

    char filename[256];
    sprintf("%s/intermidiate_%d.txt", intermediate_file, partition_number);
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < buffer.size; i++) {
        fprintf(fp, "%s\t%s\t", buffer.keys[i], buffer.values[i]);
        free(buffer.keys[i]);
        free(buffer.values[i]);
    }

    buffer.last_flush = time(NULL);
    buffer.size = 0;
}

void append_buffer(int partition_number, char *key, char *value) {
    KeyValueBuffer buffer = buffers[partition_number];
    if (buffer.size == buffer.capacity) {
        flush_buffer(partition_number);
    }
    buffer.keys[buffer.size] = key;
    buffer.values[buffer.size] = value;
    buffer.size++;
}

void check_timed_flushed() {
    printf("in check_time_flushed\n");
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
            flush_buffer(i);
        }
    }
}

char *get_next(char *key, int partition_number) {
    char *value = "a";

    return value;
}

void MR_Emit(char *key, char *value) {
    printf("%s %s\n", key, value);
    int partition_no = partitioner(key, num_partitions);

    printf("check flush\n");
    check_timed_flushed();

    printf("append buffer\n");
    append_buffer(partition_no, key, value);
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
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

    init_buffers();

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
    printf("shard_file done");
    for (int i = 0; i < s->size; i++) {
        map_worker(s->arr[i], map);
    }

    partitioner = partition;
    num_partitions = s->size;

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
    key_value *paris;  // key-value pair buffer in memory
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
