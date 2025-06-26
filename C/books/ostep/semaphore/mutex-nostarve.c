#include "common_threads.h"
#include "zemaphore.c"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//
//

int value = 0;

typedef struct __ns_mutex_t
{
  int room1;
  int room2;
  zem_t mutex;
  zem_t t1;
  zem_t t2;
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m)
{
  m->room1 = 0;
  m->room2 = 0;
  zem_init(&m->mutex, 1);
  zem_init(&m->t1, 1);
  zem_init(&m->t2, 0);
}

void ns_mutex_acquire(ns_mutex_t *m)
{
  zem_wait(&m->mutex);
  m->room1++;
  zem_post(&m->mutex);

  zem_wait(&m->t1);

  m->room2++;
  zem_wait(&m->mutex);
  m->room1--;
  if (m->room1 == 0)
  {
    zem_post(&m->mutex);
    zem_post(&m->t2);
  }
  else
  {
    zem_post(&m->mutex);
    zem_post(&m->t1);
  }

  zem_wait(&m->t2);
}

void ns_mutex_release(ns_mutex_t *m)
{
  m->room2--;
  if (m->room2 == 0)
  {
    zem_post(&m->t1);
  }
  else
  {
    zem_post(&m->t2);
  }
}

ns_mutex_t mu;

typedef struct __tinfo_t
{
  int thread_id;
} tinfo_t;

void *worker(const void *arg)
{
  tinfo_t *t = (tinfo_t *)arg;

  while (value < 10)
  {
    ns_mutex_acquire(&mu);
    printf("child %d: before %d\n", t->thread_id, value);
    value++;
    sleep(1);
    printf("child %d: after %d\n", t->thread_id, value);
    ns_mutex_release(&mu);
  }

  return NULL;
}

int main(int argc, char *argv[])
{
  assert(argc == 2);
  int num_threads = atoi(argv[1]);
  assert(num_threads > 0);

  pthread_t p[num_threads];
  tinfo_t t[num_threads];
  ns_mutex_init(&mu);

  printf("parent: begin\n");
  for (int i = 0; i < num_threads; i++)
  {
    t[i].thread_id = i;
    Pthread_create(&p[i], NULL, worker, &t[i]);
  }

  for (int i = 0; i < num_threads; i++)
  {
    Pthread_join(p[i], NULL);
  }

  printf("parent: end\n");

  return 0;
}
