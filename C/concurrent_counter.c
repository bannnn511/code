#include <pthread.h>
#include <stdio.h>

const int NUMCPUS = 2;

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
  pthread_mutex_lock(&c->llocks[thread_id]);
  c->locals[thread_id] += value;
  if (c->locals[thread_id] >= c->threshold) {
    pthread_mutex_lock(&c->glock);
    c->global += c->locals[thread_id];
    pthread_mutex_unlock(&c->glock);
    c->locals[thread_id] = 0;
  }
  pthread_mutex_unlock(&c->llocks[thread_id]);
}

int get(counter_t *c) {
  pthread_mutex_lock(&c->glock);
  int val = c->global;
  pthread_mutex_unlock(&c->glock);

  return val;
}
