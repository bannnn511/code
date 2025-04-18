//
// Created by ban on 17/4/25.
//

#ifndef HEAP_H
#define HEAP_H

#include <pthread.h>


typedef int (*heap_compare_func)(const void *a, const void *b);

typedef struct {
    void **array; // Array of void pointers to store any type
    int size;
    int capacity;
    pthread_mutex_t lock;
    heap_compare_func compare; // Function pointer for comparing elements
} heap_t;


void heap_init(heap_t *heap, int capacity, heap_compare_func compare);

void heap_insert(heap_t *heap, void *item);

void *heap_extract_min(heap_t *heap);

void heap_destroy(heap_t *heap);


#endif //HEAP_H
