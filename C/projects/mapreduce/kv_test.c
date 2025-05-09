#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "kv.h"

#include <stdlib.h>

void test_kv_append_new() {
    KeyValueStore *kv = malloc(sizeof(KeyValueStore));
    if (kv == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    init_kvs(kv);

    kv_append(kv, "key1", "value1");
    assert(kv != NULL);
    assert(strcmp(kv->kvs[0].key, "key1") == 0);
    assert(kv->kv_count == 1);
    assert(strcmp(kv->kvs[0].values[0], "value1") == 0);

    kv_append(kv, "key1", "value2");
    assert(kv->kv_count == 1);
    assert(kv->kvs[0].value_count == 2);
    assert(strcmp(kv->kvs[0].values[1], "value2") == 0);

    kv_append(kv, "key2", "value1");
    assert(kv->kv_count == 2);
    assert(kv->kvs[1].value_count == 1);
    assert(strcmp(kv->kvs[1].values[0], "value1") == 0);

    printf("New key-value append test passed\n");
    print_kv(*kv);
}

void test_kv_append_capacity() {
    KeyValueStore *kvs = malloc(sizeof(KeyValueStore));
    if (kvs == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    init_kvs(kvs);

    for (int i = 0; i < 12; i++) {
        char value[10];
        sprintf(value, "value%d", i);
        kv_append(kvs, "key1", value);
    }

    assert(kvs->kvs[0].value_capacity >= 12);
    assert(kvs->kvs[0].value_count == 12);

    printf("Capacity expansion test passed\n");
    print_kv(*kvs);
}

void test_kv_pop() {
    KeyValueStore *kvs = malloc(sizeof(KeyValueStore));
    if (kvs == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    init_kvs(kvs);

    for (int i = 0; i < 12; i++) {
        char value[10];
        sprintf(value, "value%d", i);
        kv_append(kvs, "key1", value);
    }

    assert(kvs->kvs[0].value_count == 12);


    char *popped_value = kv_pop(kvs, "key1");
    assert(popped_value != NULL);
    assert(strcmp(popped_value, "value0") == 0);
    assert(kvs->kvs[0].value_count == 11);

    popped_value = kv_pop(kvs, "key1");
    assert(popped_value != NULL);
    assert(strcmp(popped_value, "value1") == 0);
    assert(kvs->kvs[0].value_count == 10);
}

void test_kv_sort() {
    KeyValueStore *kvs = malloc(sizeof(KeyValueStore));
    if (kvs == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    init_kvs(kvs);


    kv_append(kvs, "key2", "value2");
    kv_append(kvs, "key1", "value1");
    kv_append(kvs, "key3", "value3");
    kv_append(kvs, "key5", "value5");
    kv_append(kvs, "key4", "value4");

    printf("sort\n");
    kv_sort(kvs);
    printf("sort done\n");
    assert(kvs->kvs[0].key != NULL);
    assert(strcmp(kvs->kvs[0].key, "key1") == 0);
    assert(kvs->kvs[1].key != NULL);
    assert(strcmp(kvs->kvs[1].key, "key2") == 0);
    assert(kvs->kvs[2].key != NULL);
    assert(strcmp(kvs->kvs[2].key, "key3") == 0);
    assert(kvs->kvs[3].key != NULL);
    assert(strcmp(kvs->kvs[3].key, "key4") == 0);
    assert(kvs->kvs[4].key != NULL);
    assert(strcmp(kvs->kvs[4].key, "key5") == 0);
    assert(kvs->kv_count == 5);
}

int main() {
    test_kv_append_new();
    test_kv_append_capacity();
    test_kv_pop();
    test_kv_sort();


    printf("All tests passed!\n");
    return 0;
}
