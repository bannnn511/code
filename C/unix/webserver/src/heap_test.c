#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "heap.c"

void test_init() {
    heap_t heap;
    heap_init(&heap, 10, compare_int);

    assert(heap.size == 0);
    assert(heap.capacity == 10);
    assert(heap.array != NULL);

    heap_destroy(&heap);
    printf("Init test passed\n");
}

void test_insert_one() {
    heap_t heap;
    heap_init(&heap, 10, compare_int);

    int value = 5;
    heap_insert(&heap, &value);
    assert(heap.size == 1);
    assert(*(int*)heap.array[0] == 5); // Dereference the void pointer

    heap_destroy(&heap);
    printf("Insert one test passed\n");
}

void test_insert_multiple() {
    heap_t heap;
    heap_init(&heap, 10, compare_int);

    int values[] = {5, 3, 8, 1, 9, 4};
    for (int i = 0; i < 6; i++) {
        heap_insert(&heap, &values[i]); // Pass address of each value
    }

    assert(heap.size == 6);
    assert(*(int*)heap.array[0] == 1); // Min should be at the root, dereference

    heap_destroy(&heap);
    printf("Insert multiple test passed\n");
}

void test_extract_min() {
    heap_t heap;
    heap_init(&heap, 10, compare_int);

    // Create array of values and insert their addresses
    int values[] = {5, 3, 8, 1, 9};
    heap_insert(&heap, &values[0]);
    heap_insert(&heap, &values[1]);
    heap_insert(&heap, &values[2]);
    heap_insert(&heap, &values[3]);
    heap_insert(&heap, &values[4]);

    // Extract - should come out in ascending order
    int *min;

    min = (int *) heap_extract_min(&heap);
    assert(*min == 1);

    min = (int *) heap_extract_min(&heap);
    assert(*min == 3);

    min = (int *) heap_extract_min(&heap);
    assert(*min == 5);

    min = (int *) heap_extract_min(&heap);
    assert(*min == 8);

    min = (int *) heap_extract_min(&heap);
    assert(*min == 9);

    // Should be empty now
    assert(heap.size == 0);

    // Extract from empty heap
    assert(heap_extract_min(&heap) == NULL);

    heap_destroy(&heap);
    printf("Extract min test passed\n");
}

void test_full_heap() {
    heap_t heap;
    int capacity = 5;
    heap_init(&heap, capacity, compare_int); // Add the comparison function

    // Create array for values
    int values[6] = {10, 9, 8, 7, 6, 100};

    // Fill the heap
    for (int i = 0; i < capacity; i++) {
        heap_insert(&heap, &values[i]);
    }

    assert(heap.size == capacity);

    // Try to insert one more (should be ignored)
    heap_insert(&heap, &values[5]);
    assert(heap.size == capacity);

    heap_destroy(&heap);
    printf("Full heap test passed\n");
}

void test_random_operations() {
    int test_size = 10;
    heap_t heap;
    heap_init(&heap, test_size, compare_int);

    srand(time(NULL));

    // Create two arrays - one for heap values, one for sorted reference
    int *heap_values = malloc(test_size * sizeof(int));
    int *sorted_values = malloc(test_size * sizeof(int));

    // Fill both arrays with the same random values
    for (int i = 0; i < test_size; i++) {
        int random_val = rand() % 10000;
        heap_values[i] = random_val;
        sorted_values[i] = random_val;
        heap_insert(&heap, &heap_values[i]);
    }

    // Sort only the reference array
    for (int i = 0; i < test_size; i++) {
        for (int j = i + 1; j < test_size; j++) {
            if (sorted_values[i] > sorted_values[j]) {
                int temp = sorted_values[i];
                sorted_values[i] = sorted_values[j];
                sorted_values[j] = temp;
            }
        }
    }

    // Verify extraction order
    for (int i = 0; i < test_size; i++) {
        int *min = (int *) heap_extract_min(&heap);
        printf("Extracted: %d\n", *min);
        assert(*min == sorted_values[i]);
    }

    heap_destroy(&heap);
    free(heap_values);
    free(sorted_values);
    printf("Random operations test passed\n");
}


int main() {
    test_init();
    test_insert_one();
    test_insert_multiple();
    test_extract_min();
    test_full_heap();
    test_random_operations();

    printf("All tests passed!\n");
    return 0;
}
