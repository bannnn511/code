#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

static void swim(heap_t *heap, int k);

static void sink(heap_t *heap, int k);

static void resize(heap_t *heap, int capacity);

static void exchange(void **array, int i, int j);

static int greater(heap_t *heap, int i, int j);

int compare_int(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return 0;
    }

    printf("compare %d, %d, diff %d\n", *(int *) a, *(int *) b,
           *(int *) a - *(int *) b);
    return *(int *) a - *(int *) b;
}

void heap_init(heap_t *heap, int capacity, int (*compare)(const void *, const void *)) {
    heap->array = calloc(capacity + 1, sizeof(void *)); // +1 for 1-based indexing
    heap->size = 0;
    heap->capacity = capacity;
    heap->compare = compare;
    pthread_mutex_init(&heap->lock, NULL);
}

void heap_insert(heap_t *heap, void *item) {
    pthread_mutex_lock(&heap->lock);

    // Resize if needed
    if (heap->size >= heap->capacity) {
        resize(heap, 2 * heap->capacity);
    }

    // Add item and swim up
    heap->array[++heap->size] = item;
    swim(heap, heap->size);

    pthread_mutex_unlock(&heap->lock);
}

void *heap_extract_min(heap_t *heap) {
    pthread_mutex_lock(&heap->lock);

    if (heap->size == 0) {
        pthread_mutex_unlock(&heap->lock);
        return NULL;
    }

    void *min = heap->array[1];
    exchange(heap->array, 1, heap->size--);
    sink(heap, 1);
    heap->array[heap->size + 1] = NULL; // Prevent loitering

    // Shrink if needed
    if (heap->size > 0 && heap->size == heap->capacity / 4) {
        resize(heap, heap->capacity / 2);
    }

    pthread_mutex_unlock(&heap->lock);
    return min;
}

static void swim(heap_t *heap, int k) {
    while (k > 1 && greater(heap, k / 2, k)) {
        exchange(heap->array, k / 2, k);
        k = k / 2;
    }
}

static void sink(heap_t *heap, int k) {
    while (2 * k <= heap->size) {
        int j = 2 * k;
        if (j < heap->size && greater(heap, j, j + 1)) j++;
        if (!greater(heap, k, j)) break;
        exchange(heap->array, k, j);
        k = j;
    }
}

static void resize(heap_t *heap, int capacity) {
    void **temp = calloc(capacity + 1, sizeof(void *));
    for (int i = 1; i <= heap->size; i++) {
        temp[i] = heap->array[i];
    }
    free(heap->array);
    heap->array = temp;
    heap->capacity = capacity;
}

static void exchange(void **array, int i, int j) {
    void *temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

static int greater(heap_t *heap, int i, int j) {
    return heap->compare(heap->array[i], heap->array[j]) > 0;
}

void heap_destroy(heap_t *heap) {
    pthread_mutex_lock(&heap->lock);
    free(heap->array);
    heap->array = NULL;
    heap->size = 0;
    heap->capacity = 0;
    pthread_mutex_unlock(&heap->lock);
    pthread_mutex_destroy(&heap->lock);
}

int heap_size(heap_t *heap) {
    return heap->size;
}

int heap_is_empty(heap_t *heap) {
    return heap->size == 0;
}
