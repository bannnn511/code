#include "mapreduce.h"
#include <assert.h>
#include <stdio.h>
#include <_string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "shard.h"
#include "threadpool.h"


// int shard_size = 16 * 1024; // 16kb

void map_worker(shard s, Mapper umap_func);

char *get_next(char *key, int partition_number) {
    char *value = "a";

    return value;
}

void MR_Emit(char *key, char *value) {
    printf("%s %s\n", key, value);
    // TODO: partition by keys
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
void MR_Run(int argc, char *argv[],
            Mapper map, int num_mappers,
            Reducer reduce, int num_reducers,
            Partitioner partition) {
    if (argc < 2) {
        printf("Usage: ./mapreduce <input file> <input file> ...\n");
        return;
    }

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
    for (int i = 0; i < s->size; i++) {
        map_worker(s->arr[i], map);
    }

    // // 2. start worker pools
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
    read_shard(s, buffer);
    map(buffer);
    free(buffer);
}




