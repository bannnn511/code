#ifndef KV_H
#define KV_H
#include <stdio.h>
#include <pthread.h>

typedef struct {
    char *key;
    char **values;
    size_t value_count;
    size_t value_capacity;
} KeyValue;

typedef struct {
    pthread_mutex_t mu;
    KeyValue *kvs;
    size_t kv_count;
    size_t kv_capacity;
} KeyValueStore;

void init_kvs(KeyValueStore *kv);

void kv_append(KeyValueStore *kvs, const char *key, const char *value);

void print_kv(KeyValueStore kv);

void free_kvs(KeyValueStore *kv);

char *kv_pop(const KeyValueStore *kv, const char *key);

#endif //KV_H
