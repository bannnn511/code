#include <stdlib.h>
#include "thread_helper.h"
#include "pthread.h"

// Comparison function type definition
// Returns negative if a < b, 0 if a == b, positive if a > b
typedef int (*heap_compare_func)(const void *a, const void *b);

int compare_int(const void *a, const void *b) { return (*(int *) a - *(int *) b); }

typedef struct {
    void **array; // Array of void pointers to store any type
    int size;
    int capacity;
    pthread_mutex_t lock;
    heap_compare_func compare; // Function pointer for comparing elements
} heap_t;

void heap_init(heap_t *heap, int capacity, heap_compare_func compare) {
    heap->array = (void **) malloc(capacity * sizeof(void *));
    heap->size = 0;
    heap->capacity = capacity;
    heap->compare = compare;
    thread_mutex_init(&heap->lock, NULL);
}

void min_heapify(heap_t *heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Use the compare function to determine order
    if (left < heap->size && heap->compare(heap->array[left], heap->array[smallest]) < 0) {
        smallest = left;
    }
    if (right < heap->size && heap->compare(heap->array[right], heap->array[smallest]) < 0) {
        smallest = right;
    }
    if (smallest != index) {
        void *temp = heap->array[index];
        heap->array[index] = heap->array[smallest];
        heap->array[smallest] = temp;
        min_heapify(heap, smallest);
    }
}

void heap_insert(heap_t *heap, void *item) {
    thread_mutex_lock(&heap->lock);

    if (heap->size == heap->capacity) {
        thread_mutex_unlock(&heap->lock);
        return;
    }

    // Insert at the end
    int current = heap->size;
    heap->array[current] = item;
    heap->size++;

    // Bubble up: Compare with parent and swap if needed
    while (current > 0) {
        int parent = (current - 1) / 2;
        if (heap->compare(heap->array[current], heap->array[parent]) >= 0) {
            // Current item is >= parent, heap property satisfied
            break;
        }

        // Swap with parent
        void *temp = heap->array[current];
        heap->array[current] = heap->array[parent];
        heap->array[parent] = temp;

        // Move up to parent
        current = parent;
    }

    thread_mutex_unlock(&heap->lock);
}

void *heap_extract_min(heap_t *heap) {
    thread_mutex_lock(&heap->lock);
    if (heap->size == 0) {
        thread_mutex_unlock(&heap->lock);
        return NULL;
    }
    void *min = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    min_heapify(heap, 0);
    thread_mutex_unlock(&heap->lock);
    return min;
}

void heap_destroy(heap_t *heap) {
    thread_mutex_lock(&heap->lock);
    free(heap->array);
    heap->array = NULL;
    heap->size = 0;
    heap->capacity = 0;
    thread_mutex_unlock(&heap->lock);
    pthread_mutex_destroy(&heap->lock);
}

void print_heap(heap_t *heap) {
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", *(int *) heap->array[i]);
    }
    printf("\n");
}
