//
// Created by ban on 11/12/24.
//

#include "common_threads.h"

/**
 * Semaphore implementation since MacOS does not support unnamed semaphore.
 */

typedef struct zemaphore_ {
  int value;
  pthread_cond_t cond;
  pthread_mutex_t lock;
} zem_t;

void zem_init(zem_t *z, int value) {
  z->value = value;
  Cond_init(&z->cond);
  Mutex_init(&z->lock);
}

int zem_post(zem_t *z) {
  Mutex_lock(&z->lock);
  const int tmp = z->value++;
  Cond_signal(&z->cond);
  Mutex_unlock(&z->lock);

  return tmp;
}

int zem_wait(zem_t *z) {
  Mutex_lock(&z->lock);
  while (z->value <= 0) {
    Cond_wait(&z->cond, &z->lock);
  }
  Mutex_unlock(&z->lock);
  return 0;
}
