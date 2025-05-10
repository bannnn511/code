#include "kv.h"

#include <stdio.h>
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
    if (pthread_mutex_init(&kv->mu, NULL) == -1) {
        perror("pthread_mutex_init");
        exit(EXIT_FAILURE);
    }
}

void kv_append(KeyValueStore *kv, const char *key, const char *value) {
    if (!kv) {
        return;
    }

    pthread_mutex_lock(&kv->mu);

    if (kv->kvs == NULL) {
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
        char *key_copy = strdup(key);
        char **values = malloc(sizeof(char *) * 10);
        if (!key_copy || !values) {
            free(key_copy);
            free(values);
            pthread_mutex_unlock(&kv->mu);
            return;
        }
        target->key = key_copy;
        target->values = values;
        target->value_count = 0;
        target->value_capacity = 10;
    }

    if (target->value_count == target->value_capacity) {
        target->value_capacity *= 2;
        target->values = realloc(target->values, sizeof(char *) * target->value_capacity);
    }
    char *val_copy = strdup(value);
    if (!val_copy) {
        pthread_mutex_unlock(&kv->mu);
        return;
    }
    target->values[target->value_count++] = val_copy;

    pthread_mutex_unlock(&kv->mu);
}

char *kv_pop(KeyValueStore *kv, const char *key) {
    if (!kv || !key) {
        return NULL;
    }

    pthread_mutex_lock(&kv->mu);

    for (size_t i = 0; i < kv->kv_count; i++) {
        if (strcmp(kv->kvs[i].key, key) == 0) {
            if (kv->kvs[i].value_count == 0) {
                pthread_mutex_unlock(&kv->mu);
                return NULL;
            }

            // Get the first value and make a copy for caller
            char *value = strdup(kv->kvs[i].values[0]);
            if (!value) {
                pthread_mutex_unlock(&kv->mu);
                return NULL;
            }

            // Free original and shift remaining values left
            free(kv->kvs[i].values[0]);
            for (size_t j = 0; j < kv->kvs[i].value_count - 1; j++) {
                kv->kvs[i].values[j] = kv->kvs[i].values[j + 1];
            }
            kv->kvs[i].value_count--;

            pthread_mutex_unlock(&kv->mu);
            return value;
        }
    }

    pthread_mutex_unlock(&kv->mu);

    return NULL;
}

static int compare_kv(const void *a, const void *b) {
    const KeyValue *kv1 = a;
    const KeyValue *kv2 = b;
    return strcmp(kv1->key, kv2->key);
}

void kv_sort(KeyValueStore *kv) {
    if (!kv || !kv->kvs) {
        return;
    }

    pthread_mutex_lock(&kv->mu);
    qsort(kv->kvs, kv->kv_count, sizeof(KeyValue), compare_kv);
    pthread_mutex_unlock(&kv->mu);
}

void free_kvs(KeyValueStore *kv) {
    if (!kv || !kv->kvs) {
        return;
    }

    pthread_mutex_lock(&kv->mu);

    for (size_t i = 0; i < kv->kv_count; i++) {
        if (kv->kvs[i].key) {
            free(kv->kvs[i].key);
        }
        if (kv->kvs[i].values) {
            for (size_t j = 0; j < kv->kvs[i].value_count; j++) {
                if (kv->kvs[i].values[j]) {
                    free(kv->kvs[i].values[j]);
                }
            }
            free(kv->kvs[i].values);
        }
    }
    free(kv->kvs);

    // Reset the structure but don't free kv itself
    kv->kvs = NULL;
    kv->kv_count = 0;
    kv->kv_capacity = 0;

    pthread_mutex_unlock(&kv->mu);
}

void print_kv(const KeyValueStore kv) {
    for (size_t i = 0; i < kv.kv_count; i++) {
        const KeyValue current = kv.kvs[i];
        printf("Key: %s\n", current.key);
        for (size_t j = 0; j < current.value_count; j++) {
            printf("Value %zu: %s\n", j, current.values[j]);
        }
    }
}
