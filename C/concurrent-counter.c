#include "errno.h"
#include "util.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMCPUS 10
#define ONE_MILLION 1000000

typedef struct counter_t {
  int global;                      // global count
  pthread_mutex_t glock;           // global lock
  int locals[NUMCPUS];             // per-CPU counter
  pthread_mutex_t llocks[NUMCPUS]; // local locks
  int threshold;                   // threshold
} counter_t;

// init: initialize threshold, global
//       init global lock and local lock for each threads
void init(counter_t *c, int threshold) {
  c->threshold = threshold;
  c->global = 0;
  pthread_mutex_init(&c->glock, NULL);
  int i;
  for (i = 0; i < NUMCPUS; i++) {
    c->locals[i] = 0;
    pthread_mutex_init(&c->llocks[i], NULL);
  }
}

// update: Increment the per-CPU counter for a given thread by a specified
// value.
//         If the counter exceeds the threshold, the value is added to the
//         global counter and the local counter is reset to 0. This function
//         uses locks to prevent race conditions when accessing the shared
//         global counter.
void update(counter_t *c, int thread_id, int value) {
  int cpu = thread_id % NUMCPUS;
  pthread_mutex_lock(&c->llocks[cpu]);
  c->locals[cpu] += value;
  if (c->locals[cpu] >= c->threshold) {
    pthread_mutex_lock(&c->glock);
    c->global += c->locals[cpu];
    pthread_mutex_unlock(&c->glock);
    c->locals[cpu] = 0;
  }
  pthread_mutex_unlock(&c->llocks[cpu]);
}

int get(counter_t *c) {
  pthread_mutex_lock(&c->glock);
  int val = c->global;
  pthread_mutex_unlock(&c->glock);

  return val;
}

typedef struct __myarg_t {
  counter_t *c;
  int threshold;
  int amt;
  int threads;
} myarg_t;

void *thread_function(void *arg) {
  myarg_t *m = (myarg_t *)(arg);
  pthread_t thread_id = pthread_self();

  for (int i = 0; i < ONE_MILLION; i++)
    update(m->c, (int)thread_id, m->amt);
  pthread_exit(0);
}

int main() {
  int count = 0;
  // for (int i = 1; i < NUMCPUS; i++) {
  // double threshold = pow(2, i);
  int threshold = 512;
  for (int j = 1; j <= NUMCPUS; j++) {
    count++;
    counter_t *c = malloc(sizeof(counter_t));
    if (c == NULL) {
      handle_error(errno, "malloc");
    }
    init(c, threshold);

    pthread_t *threads = malloc((size_t)j * sizeof(pthread_t));
    if (threads == NULL) {
      handle_error(errno, "malloc");
    }
    myarg_t args;
    args.c = c;
    args.threshold = (int)threshold;
    args.amt = 1;
    args.threads = j;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC,
                  &start); // Use clock_gettime instead of clock()
    for (int k = 0; k < j; k++) {
      pthread_create(&threads[k], NULL, thread_function, &args);
    }

    for (int k = 0; k < j; k++) {
      pthread_join(threads[k], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC,
                  &end); // Use clock_gettime instead of clock()me =
    double time_taken = end.tv_sec - start.tv_sec;
    time_taken +=
        (end.tv_nsec - start.tv_nsec) / 1e9; // Convert nanoseconds to seconds
    printf("%d threads, %d threshold\n", j, (int)threshold);
    printf("%d global counter\n", get(c));
    printf("Time (seconds): %f\n\n", time_taken);

    pthread_mutex_destroy(&c->glock);
    for (int m = 0; m < NUMCPUS; m++)
      pthread_mutex_destroy(&c->llocks[m]);

    free(c);
    free(threads);
    // }
  }
  printf("============ %d\n", count);

  return 0;
}