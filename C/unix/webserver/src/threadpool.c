#include <stdio.h>
#include "stdlib.h"
#include "helper.c"
#include "threadpool.h"
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>

struct worker_args {
    thread_pool *pool;
    int id;
};

void *worker(void *arg);

int thread_pool_init(thread_pool *pool, const int thread_count, const int queue_size) {
    if (thread_count <= 0 || queue_size <= 0) {
        return -1;
    }

    thread_mutex_init(&pool->lock, NULL);
    thread_cond_init(&pool->cond, NULL);
    thread_cond_init(&pool->full, NULL);
    thread_cond_init(&pool->empty, NULL);

    pool->threads = malloc(sizeof(pthread_t) * thread_count);
    pool->queue_size = queue_size;
    pool->queue = malloc(sizeof(thread_pool_task) * queue_size);
    pool->head = 0;
    pool->tail = 0;
    pool->shutdown = 0;

    for (int i = 0; i < thread_count; i++) {
        struct worker_args *args = malloc(sizeof(struct worker_args));
        if (args == NULL) {
            perror("malloc");
            return -1;
        }
        args->pool = pool;
        args->id = i;
        thread_create_or_die(&pool->threads[i], NULL, worker, args);
        pool->thread_count++;
    }

    return 0;
}

int thread_pool_add(thread_pool *pool, void (function)(void *), void *argument) {
    if (pool == NULL) {
        return -1;
    }


    thread_mutex_lock(&pool->lock);
    while (pool->task_count == pool->queue_size && !pool->shutdown) {
        printf("[pool] queue is full, waiting...\n");
        thread_cond_wait(&pool->empty, &pool->lock);
    }
    if (pool->shutdown) {
        printf("[pool] pool is shutting down, cannot add task\n");
        thread_mutex_unlock(&pool->lock);
        return -1;
    }
    printf("[pool] add task %d\n", pool->head);

    pool->queue[pool->head].function = function;
    pool->queue[pool->head].argument = argument;
    pool->head = (pool->head + 1) % pool->queue_size;
    pool->task_count++;

    thread_cond_signal(&pool->full);
    thread_mutex_unlock(&pool->lock);

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
    struct worker_args *worker_args = arg;
    thread_pool *pool = worker_args->pool;
    thread_pool_task task;

    thread_mutex_lock(&pool->lock);
    const int worker_idx = worker_args->id;
    thread_mutex_unlock(&pool->lock);


    // Set shutdown flag
    printf("[worker %d] start polling\n", worker_idx);
    for (;;) {
        printf("[worker %d] waiting for task\n", worker_idx);
        thread_mutex_lock(&pool->lock);

        while (pool->task_count == 0 && !pool->shutdown) {
            printf("[worker %d] queue is empty, waiting...\n", worker_idx);
            thread_cond_broadcast(&pool->cond);
            thread_cond_wait(&pool->full, &pool->lock);
        }

        if (pool->shutdown) {
            thread_mutex_unlock(&pool->lock);
            printf("[worker %d] shutdown\n", worker_idx);
            break;
        }


        const int idx = pool->tail;
        // printf("[worker %d] pick task %d\n", worker_idx, idx);
        task.function = pool->queue[pool->tail].function;
        task.argument = pool->queue[pool->tail].argument;

        pool->tail = (pool->tail + 1) % pool->queue_size;
        pool->task_count--;

        thread_cond_signal(&pool->empty);
        thread_mutex_unlock(&pool->lock);

        printf("[worker %d] start task %d\n", worker_idx, idx);
        const double t1 = get_seconds();
        task.function(task.argument);
        const double t2 = get_seconds();
        printf("[worker %d] finish task %d\n[worker %d] run task %d for %.2f seconds\n",
               worker_idx, idx, worker_idx, idx, t2 - t1);
    }

    printf("[worker %d] === exit ===\n", worker_idx);

    pthread_exit(NULL);
}

int thread_pool_destroy(thread_pool *pool) {
    if (pool == NULL) {
        return -1;
    }

    thread_mutex_lock(&pool->lock);
    printf("[pool] send signal shutdown\n");
    pool->shutdown = 1;
    // thread_cond_broadcast(&pool->cond);
    thread_cond_broadcast(&pool->full);
    thread_mutex_unlock(&pool->lock);
    printf("[pool] thread_pool_destroy\n");

    printf("[pool] waiting for threads to finish\n");
    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);
    free(pool->queue);

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);
    pthread_cond_destroy(&pool->full);
    pthread_cond_destroy(&pool->empty);

    return 0;
}
