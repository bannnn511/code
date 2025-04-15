#include <stdio.h>
#include "stdlib.h"
#include "helper.c"
#include "threadpool.h"
#include <fcntl.h>

void *worker(void *arg);

int thread_pool_init(thread_pool *pool, const int thread_count, const int queue_size) {
    if (thread_count <= 0 || queue_size <= 0) {
        return -1;
    }

    thread_mutex_init(&pool->lock, NULL);
    thread_cond_init(&pool->cond, NULL);

    sem_unlink("/threadpool_full"); // Clean up any existing semaphores
    sem_unlink("/threadpool_empty"); // from previous runs

    sem_t *s1 = sem_open("/threadpool_full", O_CREAT, 0644, 0);
    if (s1 == SEM_FAILED) {
        perror("sem_open");
        return -1;
    }
    pool->full = s1;

    sem_t *s2 = sem_open("/threadpool_empty", O_CREAT, 0644, queue_size);
    if (s2 == SEM_FAILED) {
        perror("sem_open");
        return -1;
    }
    pool->empty = s2;


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


    sem_wait(pool->empty);
    thread_mutex_lock(&pool->lock);

    pool->queue[pool->head].function = function;
    pool->queue[pool->head].argument = argument;
    pool->head = (pool->head + 1) % pool->queue_size;
    pool->task_count++;

    thread_mutex_unlock(&pool->lock);
    sem_post(pool->full);

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
        sem_wait(pool->full);

        thread_mutex_lock(&pool->lock);
        const int shutdown = pool->shutdown;
        thread_mutex_unlock(&pool->lock);

        printf("[worker] check shutdown %d\n", shutdown);
        if (shutdown) {
            break;
        }
        printf("[worker] pass sem full\n");
        thread_mutex_lock(&pool->lock);

        const int idx = pool->tail;
        printf("[worker] pick task %d\n", idx);
        task.function = pool->queue[pool->tail].function;
        task.argument = pool->queue[pool->tail].argument;

        pool->tail = (pool->tail + 1) % pool->queue_size;
        pool->task_count--;

        sem_post(pool->empty);
        thread_mutex_unlock(&pool->lock);

        printf("[worker] start task %d\n", idx);
        const double t1 = get_seconds();
        task.function(task.argument);
        const double t2 = get_seconds();
        printf("[worker] finish task %d\n[worker] run task %d for %.2f seconds\n", idx, idx, t2 - t1);

        thread_mutex_lock(&pool->lock);
        if (pool->task_count == 0) {
            thread_cond_signal(&pool->cond);
            printf("[worker] signal all tasks finished\n");
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
    pool->shutdown = 1;
    thread_cond_broadcast(&pool->cond);
    thread_mutex_unlock(&pool->lock);
    printf("[pool] thread_pool_destroy\n");

    while (pool->task_count > 0) {
        thread_cond_wait(&pool->cond, &pool->lock);
    }

    // wait for all threads to finish
    for (int i = 0; i < pool->thread_count; i++) {
        sem_post(pool->full);
    }

    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);
    free(pool->queue);

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);
    sem_close(pool->full);
    sem_close(pool->empty);
    sem_unlink("/threadpool_full");
    sem_unlink("/threadpool_empty");

    return 0;
}
