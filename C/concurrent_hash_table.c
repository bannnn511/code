#define BUCKET (2)

#include "cocurrent_queue.c"

typedef struct __hash_t {
  queue_t lists[BUCKET];
} hash_t;

void Hash_Init(hash_t *h) {
  for (int i = 0; i < BUCKET; i++) {
    Queue_init(&h->lists[i]);
  }
}

void Hash_Insert(hash_t *h, int key) {
  int bucket = key % BUCKET;
  Queue_Enqueue(&h->lists[bucket], key);
}

int Hash_Lookup(hash_t h, int key) {
  int bucket = key % BUCKET;
  return Queue_Dequeue(&h.lists[bucket]);
}
