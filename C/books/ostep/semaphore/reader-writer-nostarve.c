#include "common_threads.h"
#include "zemaphore.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//
// Your code goes in the structure and functions below
//

typedef struct __rwlock_t
{
  int readers;
  zem_t turnstile;
  zem_t rlock;
  zem_t wlock;
} rwlock_t;

void rwlock_init(rwlock_t *rw)
{
  zem_init(&rw->rlock, 1);
  zem_init(&rw->wlock, 1);
  zem_init(&rw->turnstile, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw)
{
  zem_wait(&rw->turnstile); // block readers if there are waiting writers
  zem_post(&rw->turnstile);

  zem_wait(&rw->rlock);
  rw->readers++;
  if (rw->readers == 1)
  {
    zem_wait(&rw->wlock);
  }
  zem_post(&rw->rlock);
}

void rwlock_release_readlock(rwlock_t *rw)
{
  zem_wait(&rw->rlock);
  rw->readers--;
  if (rw->readers == 0)
  {
    zem_post(&rw->wlock);
  }
  zem_post(&rw->rlock);
}

void rwlock_acquire_writelock(rwlock_t *rw)
{
  zem_wait(&rw->turnstile);
  zem_wait(&rw->wlock);
}

void rwlock_release_writelock(rwlock_t *rw)
{
  zem_post(&rw->turnstile);
  zem_post(&rw->wlock);
}

//
// Don't change the code below (just use it!)
//

int loops;
int value = 0;

rwlock_t lock;

void *reader(void *arg)
{
  int i;
  for (i = 0; i < loops; i++)
  {
    rwlock_acquire_readlock(&lock);
    printf("read %d\n", value);
    sleep(1);
    rwlock_release_readlock(&lock);
  }
  return NULL;
}

void *writer(void *arg)
{
  int i;
  for (i = 0; i < loops; i++)
  {
    rwlock_acquire_writelock(&lock);
    value++;
    printf("write %d\n", value);
    sleep(1);
    rwlock_release_writelock(&lock);
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  assert(argc == 4);
  int num_readers = atoi(argv[1]);
  int num_writers = atoi(argv[2]);
  loops = atoi(argv[3]);

  pthread_t pr[num_readers], pw[num_writers];

  rwlock_init(&lock);

  printf("begin\n");

  int i;
  for (i = 0; i < num_readers; i++)
    Pthread_create(&pr[i], NULL, reader, NULL);
  for (i = 0; i < num_writers; i++)
    Pthread_create(&pw[i], NULL, writer, NULL);

  for (i = 0; i < num_readers; i++)
    Pthread_join(pr[i], NULL);
  for (i = 0; i < num_writers; i++)
    Pthread_join(pw[i], NULL);

  printf("end: value %d\n", value);

  return 0;
}
