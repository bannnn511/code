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

// int main() {
//   hash_t myHashTable;
//   Hash_Init(&myHashTable); // Initialize the hash table

//   // Test Hash_Insert
//   Hash_Insert(&myHashTable, 5); // Insert key 5
//   Hash_Insert(&myHashTable,
//               205); // Insert key 205, should go to the same bucket as 5
//   Hash_Insert(&myHashTable, 42); // Insert key 42

//   // Test Hash_Lookup
//   int found1 = Hash_Lookup(myHashTable, 5);   // Lookup key 5
//   int found2 = Hash_Lookup(myHashTable, 205); // Lookup key 205
//   int found3 = Hash_Lookup(myHashTable, 42);  // Lookup key 42

//   printf("Lookup key 5: %d\n", found1);
//   printf("Lookup key 205: %d\n", found2);
//   printf("Lookup key 42: %d\n", found3);
// }