#include "util.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMCPUS 10
#define ONE_MILLION 10000000

typedef struct __counter_t {
  pthread_mutex_t lock;
  int value;
} counter_t;

static void init(counter_t *c) {
  c->value = 0;
  pthread_mutex_init(&c->lock, NULL);
}

static void increment(counter_t *c) {
  pthread_mutex_lock(&c->lock);
  c->value++;
  pthread_mutex_unlock(&c->lock);
}

static int get(counter_t *c) {
  pthread_mutex_lock(&c->lock);
  int rc = c->value;
  pthread_mutex_unlock(&c->lock);
  return rc;
}

typedef struct __myarg_t {
  counter_t *counter;
} myarg_t;

static void *thread_function(void *arg) {
  myarg_t *m = (myarg_t *)arg;
  for (int i = 0; i < ONE_MILLION; i++)
    increment(m->counter);

  pthread_exit(0);
}

int main() {
  long num_cpus;

  num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
  if (num_cpus < 1) {
    perror("sysconf");
    return -1;
  }

  printf("Number of CPUs: %ld\n", num_cpus);
  int count = 0;
  //   for (int i = 1; i < NUMCPUS; i++) {
  for (int j = 1; j <= NUMCPUS; j++) {
    count++;
    counter_t *c = malloc(sizeof(counter_t));
    if (c == NULL) {
      handle_error(errno, "malloc");
    }
    init(c);

    pthread_t *threads = malloc((size_t)j * sizeof(pthread_t));
    if (threads == NULL) {
      handle_error(errno, "malloc");
    }

    myarg_t *args = malloc((size_t)j * sizeof(myarg_t));
    if (args == NULL) {
      handle_error(errno, "malloc");
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC,
                  &start); // Use clock_gettime instead of clock()

    for (int k = 0; k < j; k++) {
      args[k].counter = c;
      pthread_create(&threads[k], NULL, &thread_function, &args[k]);
    }

    for (int k = 0; k < j; k++) {
      pthread_join(threads[k], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC,
                  &end); // Use clock_gettime instead of clock()me =
    double time_taken = end.tv_sec - start.tv_sec;
    time_taken +=
        (end.tv_nsec - start.tv_nsec) / 1e9; // Convert nanoseconds to seconds
    printf("%d global counter\n", get(c));
    printf("Time (seconds): %f\n\n", time_taken);

    pthread_mutex_destroy(&c->lock);
    free(c);
    free(args);
    //   }
  }
  printf("============ %d\n", count);

  return 0;
}