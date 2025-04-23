#include "mapreduce.h"
#include <stdio.h>
#include "task_queue.h"


task_queue queue;

char *get_next(char *key, int partition_number) {
    char *v = "a";
    return v;
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
    init_task_queue(&queue);
    for (int i = 1; i < argc; i++) {
        char *file_name = argv[i];
        map(file_name);
    }

    for (;;) {
        char *key;
        char *value;
        int i = dequeue_task(&queue, &key, &value);
        if (i == -1) {
            break;
        }
        reduce(key, get_next, 0);
    }
    printf("done\n");
}


