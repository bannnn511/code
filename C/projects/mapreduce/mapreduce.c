#include "mapreduce.h"

#include <assert.h>
#include <stdio.h>
#include <tgmath.h>
#include <_string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "task_queue.h"


// int shard_size = 16 * 1024; // 16kb
task_queue queue;

char *get_next(char *key, int partition_number) {
    char *value;
    int i = dequeue_task(&queue, &key, &value);
    if (i == -1) {
        return NULL;
    }

    return value;
}

void MR_Emit(char *key, char *value) {
    enqueue_task(&queue, key, value);
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0') {
        hash = hash * 33 + c;
    }

    return hash % num_partitions;
}

/**
 * 1. split the input files into M pieces
 */
void MR_Run(int argc, char *argv[],
            Mapper map, int num_mappers,
            Reducer reduce, int num_reducers,
            Partitioner partition) {
    if (argc < 2) {
        printf("Usage: ./mapreduce <input file> <input file> ...\n");
        return;
    }

    init_task_queue(&queue);
    for (int i = 1; i < argc; i++) {
        char *file_name = argv[i];
        map(file_name);
    }

    while (queue.size > 0) {
        reduce(NULL, get_next, 0);
    }
    printf("done\n");
}





