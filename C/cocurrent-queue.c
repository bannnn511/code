#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct __node_t {
  int value;
  struct __node_t *next;
} node_t;

typedef struct __queue_t {
  node_t *head;
  node_t *tail;
  pthread_mutex_t head_lock, tail_lock;
} queue_t;

void Queue_init(queue_t *q) {
  node_t *tmp = malloc(sizeof(node_t));
  tmp->next = NULL;
  q->head = q->tail = tmp;

  pthread_mutex_init(&q->head_lock, NULL);
  pthread_mutex_init(&q->head_lock, NULL);
}

void Queue_Enqueue(queue_t *q, int value) {
  node_t *tmp = malloc(sizeof(node_t));
  tmp->value = value;
  tmp->next = NULL;

  pthread_mutex_lock(&q->tail_lock);
  q->tail->next = tmp;
  q->tail = tmp;
  pthread_mutex_unlock(&q->tail_lock);
}

int Queue_Dequeue(queue_t *q) {
  pthread_mutex_lock(&q->head_lock);
  node_t *tmp = q->head;
  node_t *new_head = tmp->next;
  if (new_head == NULL) {
    pthread_mutex_unlock(&q->head_lock);
    return -1;
  }

  q->head = new_head;
  int val = new_head->value;
  pthread_mutex_unlock(&q->head_lock);
  free(tmp);

  return val;
}

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