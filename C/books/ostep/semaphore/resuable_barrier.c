#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "zemaphore.c"

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

// You likely need two semaphores to do this correctly, and some
// other integers to track things.

typedef struct __barrier_t
{
    int n; // number of threads
    int count;
    zem_t mutex;
    zem_t turnstile1;
    zem_t turnstile2;
} barrier_t;

// the single barrier we are using for this program
barrier_t b;

void barrier_init(barrier_t *b, int num_threads)
{
    b->n = num_threads;
    b->count = 0;
    zem_init(&b->mutex, 1);
    zem_init(&b->turnstile1, 0);
    zem_init(&b->turnstile2, 1);
}

void barrier(barrier_t *b, int thread_id)
{
    zem_wait(&b->mutex);
    b->count++;
    if (b->count == b->n)
    {
        zem_wait(&b->turnstile2);
        zem_post(&b->turnstile1);
    }
    zem_post(&b->mutex);

    zem_wait(&b->turnstile1);
    zem_post(&b->turnstile1);

    printf("Thread %d critical point\n", thread_id);

    zem_wait(&b->mutex);
    b->count--;
    if (b->count == 0)
    {
        zem_wait(&b->turnstile1);
        zem_post(&b->turnstile2);
    }
    zem_post(&b->mutex);

    zem_wait(&b->turnstile2);
    zem_post(&b->turnstile2);

    printf("Thread %d exit\n", thread_id);
}

//
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t
{
    int thread_id;
    int num_barriers;
} tinfo_t;

void *child(void *arg)
{
    tinfo_t *t = (tinfo_t *)arg;
    for (int i = 0; i < t->num_barriers; i++)
    {
        printf("child %d: before\n", t->thread_id);
        barrier(&b, t->thread_id);
        sleep(1);
        printf("child %d: after\n", t->thread_id);
    }
    return NULL;
}

// Modified main function
int main(int argc, char *argv[])
{
    assert(argc == 3);
    int num_threads = atoi(argv[1]);
    int num_barriers = atoi(argv[2]);
    assert(num_threads > 0);
    assert(num_barriers > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    barrier_init(&b, num_threads);

    // Create threads

    printf("parent: begin\n");
    for (int i = 0; i < num_threads; i++)
    {
        t[i].thread_id = i;
        t[i].num_barriers = num_barriers;
        Pthread_create(&p[i], NULL, child, &t[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < num_threads; i++)
    {
        Pthread_join(p[i], NULL);
    }
    printf("parent: end\n");

    return 0;
}
