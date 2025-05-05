#include "kv.h"

#include <stdlib.h>
#include <string.h>

void init_kvs(KeyValueStore *kv) {
    if (!kv) {
        fprintf(stderr, "kv is null\n");
        return;
    }
    kv->kvs = NULL;
    kv->kv_count = 0;
    kv->kv_capacity = 10;
}

void kv_append(KeyValueStore *kv, const char *key, const char *value) {
    if (!kv) {
        return;
    }

    if (!kv->kvs) {
        kv->kvs = malloc(sizeof(KeyValue) * kv->kv_capacity);
        kv->kv_count = 0;
    }

    // Search for an existing key
    KeyValue *target = NULL;
    for (size_t i = 0; i < kv->kv_count; i++) {
        if (strcmp(kv->kvs[i].key, key) == 0) {
            target = &kv->kvs[i];
            break;
        }
    }

    if (!target) {
        if (kv->kv_count == kv->kv_capacity) {
            kv->kv_capacity *= 2;
            kv->kvs = realloc(kv->kvs, sizeof(KeyValue) * kv->kv_capacity);
        }
        target = &kv->kvs[kv->kv_count++];
        target->key = strdup(key);
        target->values = malloc(sizeof(char *) * 10);
        target->value_count = 0;
        target->value_capacity = 10;
    }

    if (target->value_count == target->value_capacity) {
        target->value_capacity *= 2;
        target->values = realloc(target->values, sizeof(char *) * target->value_capacity);
    }
    target->values[target->value_count++] = strdup(value);
}

void free_kvs(KeyValueStore *kv) {
    if (!kv) {
        return;
    }
    for (size_t i = 0; i < kv->kv_count; i++) {
        free(kv->kvs[i].key);
        for (size_t j = 0; j < kv->kvs[i].value_count; j++) {
            free(kv->kvs[i].values[j]);
        }
        free(kv->kvs[i].values);
    }
    free(kv->kvs);
    free(kv);
}

void print_kv(const KeyValueStore kv) {
    for (int i = 0;i < kv.kv_count; i++) {
        const KeyValue current = kv.kvs[i];
        printf("Key: %s\n", current.key);
        for (size_t j = 0; j < current.value_count; j++) {
            printf("Value %zu: %s\n", j, current.values[j]);
        }
    }
}
