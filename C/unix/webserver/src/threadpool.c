#include <stdio.h>
#include "stdlib.h"
#include "helper.c"
#include "threadpool.h"


void *worker(void *arg);

int thread_pool_init(thread_pool *pool, const int thread_count, const int queue_size) {
    thread_mutex_init(&pool->lock, NULL);
    thread_cond_init(&pool->cond, NULL);

    if (thread_count <= 0 || queue_size <= 0) {
        return -1;
    }
    pool->threads = malloc(sizeof(pthread_t) * thread_count);
    pool->queue_size = queue_size;
    pool->queue = malloc(sizeof(thread_pool_task) * queue_size);
    pool->head = 0;
    pool->tail = 0;

    for (int i = 0; i < thread_count; i++) {
        thread_create_or_die(&pool->threads[i], NULL, worker, pool);
        pool->thread_count++;
    }

    return 0;
}

int thread_pool_add(thread_pool *pool, void (function)(void *), void *argument) {
    printf("[pool] add task\n");
    if (pool == NULL) {
        return -1;
    }

    thread_mutex_lock(&pool->lock);
    if (pool->task_count == pool->queue_size) {
        thread_mutex_unlock(&pool->lock);
        return -1;
    }

    pool->queue[pool->head].function = function;
    pool->queue[pool->head].argument = argument;
    pool->head = (pool->head + 1) % pool->queue_size;
    pool->task_count++;
    thread_mutex_unlock(&pool->lock);

    thread_cond_signal(&pool->cond);

    return 0;
}

void thread_pool_wait(thread_pool *pool) {
    printf("[pool] thread_pool_wait\n");
    if (pool == NULL) {
        return;
    }

    thread_mutex_lock(&pool->lock);
    while (pool->task_count > 0) {
        thread_cond_wait(&pool->cond, &pool->lock);
    }
    thread_mutex_unlock(&pool->lock);
    printf("[pool] finishes all tasks\n");
}

void *worker(void *arg) {
    printf("[worker] start running\n");
    thread_pool *pool = arg;
    thread_pool_task task;
    for (;;) {
        thread_mutex_lock(&pool->lock);
        while (pool->task_count == 0) {
            thread_cond_wait(&pool->cond, &pool->lock);
        }

        int idx = pool->tail;
        printf("[worker] pick task %d\n", idx);
        task.function = pool->queue[pool->tail].function;
        task.argument = pool->queue[pool->tail].argument;

        pool->tail = (pool->tail + 1) % pool->queue_size;
        pool->task_count--;
        thread_mutex_unlock(&pool->lock);

        printf("[worker] start task %d\n", idx);
        const double t1 = get_seconds();
        task.function(task.argument);
        const double t2 = get_seconds();
        printf("[worker] finish task %d\n[worker] run task for %.2f seconds\n", idx, t2 - t1);

        thread_mutex_lock(&pool->lock);
        if (pool->task_count == 0) {
            thread_cond_signal(&pool->cond);
        }
        thread_mutex_unlock(&pool->lock);
    }

    printf("[worker] exit\n");

    pthread_exit(NULL);
}

int thread_pool_destroy(thread_pool *pool) {
    if (pool == NULL) {
        return -1;
    }

    thread_mutex_lock(&pool->lock);
    thread_cond_broadcast(&pool->cond);
    thread_mutex_unlock(&pool->lock);

    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);
    free(pool->queue);

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);

    return 0;
}
