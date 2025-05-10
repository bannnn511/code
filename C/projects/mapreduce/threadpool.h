//
// Created by ban on 12/4/25.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "heap.h"

typedef struct thread_pool_task {
    void (*function)(void *);

    void *argument;
    int file_size;
    int conn_fd;
} thread_pool_task;

typedef enum thread_pool_policy {
    THREAD_POOL_POLICY_FIFO = 0, // First-in, first-out scheduling
    THREAD_POOL_POLICY_SFF, // Smallest file first
} thread_pool_policy;

typedef struct thread_pool {
    pthread_mutex_t lock;
    pthread_cond_t full;
    pthread_cond_t empty;
    pthread_cond_t cond;
    thread_pool_task *queue; // for FIFO queue
    heap_t *sff_queue; // for smallest file first queue
    pthread_t *threads;
    int queue_size;
    int task_count;      // Number of tasks in queue
    int active_tasks;    // Number of tasks currently being executed
    int thread_count;
    int head;
    int tail;
    int shutdown;
    int idx;
    thread_pool_policy policy;
} thread_pool;

int thread_pool_init(thread_pool *, int thread_count, int queue_size, thread_pool_policy);

int thread_pool_add(thread_pool *pool, void (*function)(void *), void *argument);

void thread_pool_wait(thread_pool *pool);

int thread_pool_destroy(thread_pool *pool);

#endif  // THREADPOOL_H
