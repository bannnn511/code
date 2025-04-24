#include <stdio.h>
#include <assert.h>
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
    assert(*(int*)heap.array[1] == 5); // Dereference the void pointer

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
    assert(*(int*)heap.array[1] == 1); // Min should be at the root, dereference

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
    assert(heap.size == capacity+1);
    assert(heap.capacity == capacity*2);

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
        assert(*min == sorted_values[i]);
    }

    heap_destroy(&heap);
    free(heap_values);
    free(sorted_values);
    printf("Random operations test passed\n");
}

void test_multiple_insert_extract() {
    heap_t heap;
    heap_init(&heap, 10, compare_int);

    // Test case 1: Insert ascending order
    printf("Testing ascending order insertions...\n");
    int asc[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        heap_insert(&heap, &asc[i]);
    }
    // Verify min extraction in ascending order
    for (int i = 0; i < 5; i++) {
        int *min = (int *) heap_extract_min(&heap);
        assert(*min == i + 1);
    }
    assert(heap_is_empty(&heap));

    // Test case 2: Insert descending order
    printf("Testing descending order insertions...\n");
    int desc[] = {5, 4, 3, 2, 1};
    for (int i = 0; i < 5; i++) {
        heap_insert(&heap, &desc[i]);
    }
    // Verify min extraction in ascending order
    for (int i = 0; i < 5; i++) {
        int *min = (int *) heap_extract_min(&heap);
        assert(*min == i + 1);
    }
    assert(heap_is_empty(&heap));

    // Test case 3: Mixed operations
    printf("Testing mixed operations...\n");
    int values[] = {7, 3, 9, 1, 5};
    // Insert first three values
    heap_insert(&heap, &values[0]); // 7
    heap_insert(&heap, &values[1]); // 3
    heap_insert(&heap, &values[2]); // 9
    // Extract min (should be 3)
    int *min = (int *) heap_extract_min(&heap);
    assert(*min == 3);
    // Insert two more values
    heap_insert(&heap, &values[3]); // 1
    heap_insert(&heap, &values[4]); // 5

    // Extract all remaining values (should come out as 1,5,7,9)
    int expected[] = {1, 5, 7, 9};
    for (int i = 0; i < 4; i++) {
        min = (int *) heap_extract_min(&heap);
        assert(*min == expected[i]);
    }

    assert(heap_is_empty(&heap));
    assert(heap_extract_min(&heap) == NULL);

    heap_destroy(&heap);
    printf("Multiple insert/extract test passed\n");
}

void test_custome_case() {
    heap_t heap;
    heap_init(&heap, 10, compare_int);

    // Test case 1: Insert ascending order
    printf("Testing ascending order insertions...\n");
    int asc[] = {0, 80, 0};
    for (int i = 0; i < 3; i++) {
        heap_insert(&heap, &asc[i]);
    }
    // Verify min extraction in ascending order
    int *min;
    min = (int *) heap_extract_min(&heap);
    assert(*min ==0);
    min = (int *) heap_extract_min(&heap);
    assert(*min ==0);
    min = (int *) heap_extract_min(&heap);
    assert(*min ==80);
}


int main() {
    // test_init();
    // test_insert_one();
    // test_insert_multiple();
    // test_extract_min();
    // test_full_heap();
    // test_random_operations();
    // test_multiple_insert_extract();
    test_custome_case();

    printf("All tests passed!\n");
    return 0;
}
