//
// Created by ban on 12/4/25.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "thread_helper.h"
#include <pthread.h>
#include <semaphore.h>

typedef struct thread_pool_task {
    void (*function)(void *);

    void *argument;
} thread_pool_task;

typedef struct thread_pool {
    pthread_mutex_t lock;
    pthread_cond_t full;
    pthread_cond_t empty;
    pthread_cond_t cond;
    thread_pool_task *queue;
    pthread_t *threads;
    int queue_size;
    int task_count;
    int thread_count;
    int head;
    int tail;
    int shutdown;
    int idx;
} thread_pool;

int thread_pool_init(thread_pool *pool, const int thread_count, const int queue_size);

int thread_pool_add(thread_pool *pool, void (*function)(void *), void *argument);

void thread_pool_wait(thread_pool *pool);

int thread_pool_destroy(thread_pool *pool);

#endif  // THREADPOOL_H
