#include "concurrent_hash_table.c"

int main() {
  hash_t myHashTable;
  Hash_Init(&myHashTable); // Initialize the hash table

  // Test Hash_Insert
  Hash_Insert(&myHashTable, 5); // Insert key 5
  Hash_Insert(&myHashTable,
              205); // Insert key 205, should go to the same bucket as 5
  Hash_Insert(&myHashTable, 42); // Insert key 42

  // Test Hash_Lookup
  int found1 = Hash_Lookup(myHashTable, 5);   // Lookup key 5
  int found2 = Hash_Lookup(myHashTable, 205); // Lookup key 205
  int found3 = Hash_Lookup(myHashTable, 42);  // Lookup key 42

  printf("Lookup key 5: %d\n", found1);
  printf("Lookup key 205: %d\n", found2);
  printf("Lookup key 42: %d\n", found3);
}

// int main() {
//   counter_t myCounter;
//   init(&myCounter, 5); // Initialize with a threshold of 10

//   update(&myCounter, 0, 5); // Update thread 0 with value 5
//   update(&myCounter, 1, 3); // Update thread 1 with value 3
//   update(&myCounter, 0,
//          6); // Update thread 0 with value 6, should trigger a global update

//   // Test get
//   int globalCount = get(&myCounter);
//   printf("Global count after updates: %d\n",
//          globalCount); // Should print the global count

//   return 0;
// }

/*


int main() {
  queue_t myQueue;
  Queue_init(&myQueue); // Initialize the queue

  // Test enqueue
  Queue_Enqueue(&myQueue, 10); // Enqueue value 10
  Queue_Enqueue(&myQueue, 20); // Enqueue value 20

  // Test dequeue
  int firstValue = Queue_Dequeue(&myQueue);  // Should dequeue value 10
  int secondValue = Queue_Dequeue(&myQueue); // Should dequeue value 20
  int emptyDequeue =
      Queue_Dequeue(&myQueue); // Should return -1 as queue is empty

  printf("First dequeued value: %d\n", firstValue);
  printf("Second dequeued value: %d\n", secondValue);
  printf("Dequeue on empty queue: %d\n", emptyDequeue);
}
*/