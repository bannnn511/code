#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include<assert.h>
#include "util.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>       

#define ONE_MILLION 1000000

typedef struct __node_t {
  struct __node_t *next;
  int key;
  char pad[(sizeof(struct __node_t *)) - sizeof(int)];
} node_t;

typedef struct __list_t {
  node_t *head;
  pthread_mutex_t lock;
} list_t;

static void List_Init(list_t *l) {
  l->head = NULL;
  pthread_mutex_init(&l->lock, NULL);
}

int List_Insert(list_t *l, int value) {
  node_t *node = malloc(sizeof(node_t));
  if (node == NULL) {
    handle_error(errno, "malloc");
    return -1;
  }

  node->key = value;
  pthread_mutex_lock(&l->lock);
  node->next = l->head;
  l->head = node;
  pthread_mutex_unlock(&l->lock);

  return 0;
}

int List_Lookup(list_t *l, int value) {
  int rv = -1;
  pthread_mutex_lock(&l->lock);
  node_t *curr = l->head;
  while (curr) {
    if (curr->key == value) {
      rv = 0;
      break;
    }

    curr = curr->next;
  }
  pthread_mutex_unlock(&l->lock);

  return rv;
}

static void List_Free(list_t *L) {
  pthread_mutex_lock(&L->lock);
  node_t *curr = L->head;
  while (curr) {
    node_t *temp = curr;
    curr = curr->next;
    free(temp);
  }
  pthread_mutex_unlock(&L->lock);
  free(L);
}

void *thread_function(void *args) {
  list_t *l = (list_t *)args;
  List_Lookup(l, 100);
  pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  assert(argc==3);
  int list_length = atoi(argv[1]);
  int thread_count = atoi(argv[2]);

  list_t *list = malloc(sizeof(list_t));
  if (list == NULL) {
    handle_error(errno, "malloc");
  }
  List_Init(list);

  for (int i = 0; i < list_length; i++)
    List_Insert(list, i);

  for (int i = 1; i <= thread_count; i++) {
    struct timespec start, end;

    pthread_t *threads = malloc((size_t)i * sizeof(pthread_t));
    if (threads == NULL) {
      handle_error(errno, "malloc");
    }

    clock_gettime(CLOCK_MONOTONIC,
                  &start); // Use clock_gettime instead of clock()
    for (int j = 0; j < i; j++)
      pthread_create(&threads[j], NULL, &thread_function, list);
    for (int k = 0; k < i; k++)
      pthread_join(threads[k], NULL);

    clock_gettime(CLOCK_MONOTONIC,
                  &end); // Use clock_gettime instead of clock()me =
    double time_taken = end.tv_sec - start.tv_sec;
    time_taken +=
        (end.tv_nsec - start.tv_nsec) / 1e9; // Convert nanoseconds to seconds

    printf("%d threads, time (seconds): %f\n\n", i, time_taken);

    free(threads);
  }

  List_Free(list);

  return 0;
}
