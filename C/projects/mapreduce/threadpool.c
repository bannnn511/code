#include <stdio.h>
#include "stdlib.h"
#include "threadpool.h"
#include "heap.h"
#include <sys/stat.h>
#include "thread_helper.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>

struct worker_args {
    thread_pool *pool;
    int id;
};

void *worker(void *arg);

off_t check_file_size(const int fd);

int compare_task(const void *a, const void *b);

int pick_task(thread_pool *pool, thread_pool_task *task);

void add_task(thread_pool *pool, thread_pool_task task);

double get_seconds(void);

int thread_pool_init(thread_pool *pool, const int thread_count, const int queue_size,
                     thread_pool_policy policy) {
    if (thread_count <= 0 || queue_size <= 0) {
        return -1;
    }

    thread_mutex_init(&pool->lock, NULL);
    thread_cond_init(&pool->cond, NULL);
    thread_cond_init(&pool->full, NULL);
    thread_cond_init(&pool->empty, NULL);

    pool->threads = malloc(sizeof(pthread_t) * thread_count);
    pool->queue_size = queue_size;
    pool->head = 0;
    pool->tail = 0;
    pool->shutdown = 0;
    pool->policy = policy;
    pool->thread_count = 0;
    pool->task_count = 0;
    pool->active_tasks = 0;
    pool->policy = policy;

    if (policy == THREAD_POOL_POLICY_SFF) {
        pool->sff_queue = malloc(sizeof(heap_t));
        heap_init(pool->sff_queue, queue_size, compare_task);
    } else if (policy == THREAD_POOL_POLICY_FIFO) {
        pool->queue = malloc(sizeof(thread_pool_task) * queue_size);
    } else {
        fprintf(stderr, "Invalid thread pool policy\n");
        return -1;
    }

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

int thread_pool_add(thread_pool *pool, void(function)(void *), void *argument) {
    if (pool == NULL) {
        return -1;
    }

    thread_mutex_lock(&pool->lock);
    while (pool->task_count == pool->queue_size && !pool->shutdown) {
        // printf("[pool] queue is full, waiting...\n");
        thread_cond_wait(&pool->empty, &pool->lock);
    }

    if (pool->shutdown) {
        // printf("[pool] pool is shutting down, cannot add task\n");
        thread_mutex_unlock(&pool->lock);
        return -1;
    }

    thread_pool_task task = {};
    task.function = function;
    task.argument = argument;
    add_task(pool, task);

    thread_cond_signal(&pool->full);
    thread_mutex_unlock(&pool->lock);

    return 0;
}

void thread_pool_wait(thread_pool *pool) {
    // printf("[pool] thread_pool_wait\n");
    if (pool == NULL) {
        return;
    }

    thread_mutex_lock(&pool->lock);
    while (pool->task_count > 0 || pool->active_tasks > 0) {
        thread_cond_wait(&pool->cond, &pool->lock);
    }
    thread_mutex_unlock(&pool->lock);
    // printf("[pool] finishes all tasks\n");
}

void *worker(void *arg) {
    const struct worker_args *worker_args = arg;
    thread_pool *pool = worker_args->pool;

    thread_mutex_lock(&pool->lock);
    const int worker_idx = worker_args->id;
    thread_mutex_unlock(&pool->lock);

    // printf("[worker %d] start polling\n", worker_idx);
    for (;;) {
        thread_mutex_lock(&pool->lock);

        while (pool->task_count == 0 && !pool->shutdown) {
            thread_cond_broadcast(&pool->cond);
            thread_cond_wait(&pool->full, &pool->lock);
        }

        if (pool->shutdown) {
            thread_mutex_unlock(&pool->lock);
            break;
        }

        thread_pool_task *task = malloc(sizeof(thread_pool_task));
        if (task == NULL) {
            perror("malloc");
            thread_mutex_unlock(&pool->lock);
            break;
        }
        pick_task(pool, task);
        // printf("worker %d pick task %d, file_size %d\n", worker_idx, task->conn_fd,
        // task->file_size);

        pool->active_tasks++;
        thread_cond_signal(&pool->empty);
        thread_mutex_unlock(&pool->lock);

        // printf("[worker %d] start task\n", worker_idx);
        // const double t1 = get_seconds();
        if (task->function != NULL) {
            task->function(task->argument);
        } else {
            fprintf(stderr, "[worker %d] Invalid function pointer\n", worker_idx);
        }
        // const double t2 = get_seconds();
        // printf("[worker %d] run task for %.2f seconds\n", worker_idx, t2 - t1);

        thread_mutex_lock(&pool->lock);
        pool->active_tasks--;
        thread_cond_broadcast(&pool->cond);
        thread_mutex_unlock(&pool->lock);

        free(task);
    }

    // printf("[worker %d] === exit ===\n", worker_idx);

    free(arg);
    pthread_exit(NULL);
}

int thread_pool_destroy(thread_pool *pool) {
    if (pool == NULL) {
        return -1;
    }

    thread_mutex_lock(&pool->lock);
    // printf("[pool] send signal shutdown\n");
    pool->shutdown = 1;
    thread_cond_broadcast(&pool->full);
    thread_mutex_unlock(&pool->lock);
    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);
    if (pool->policy == THREAD_POOL_POLICY_SFF) {
        heap_destroy(pool->sff_queue);
        free(pool->sff_queue);
    }
    free(pool->queue);

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);
    pthread_cond_destroy(&pool->full);
    pthread_cond_destroy(&pool->empty);

    return 0;
}

off_t check_file_size(const int fd) {
    struct stat st = {};
    if (fstat(fd, &st) < 0) {
        fprintf(stderr, "fstat failed\n");
        return 0;
    }

    return st.st_size;
}

int compare_task(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return 0;
    }
    const thread_pool_task *task1 = *(thread_pool_task **)a;
    const thread_pool_task *task2 = *(thread_pool_task **)b;

    return task1->file_size - task2->file_size;
}

void add_task(thread_pool *pool, thread_pool_task task) {
    switch (pool->policy) {
        case THREAD_POOL_POLICY_FIFO:
            pool->queue[pool->head] = task;
            pool->head = (pool->head + 1) % pool->queue_size;
            break;
        case THREAD_POOL_POLICY_SFF:
            thread_pool_task *new_task = malloc(sizeof(thread_pool_task));
            if (new_task == NULL) {
                perror("malloc");
                return;
            }
            const intptr_t conn_fd = (intptr_t)task.argument;
            new_task->function = task.function;
            new_task->argument = task.argument;
            new_task->file_size = check_file_size(conn_fd);
            new_task->conn_fd = conn_fd;
            printf("add task: %d, file size: %d\n", (int)conn_fd, new_task->file_size);
            heap_insert(pool->sff_queue, new_task);
            break;
    }
    pool->task_count++;
}

int pick_task(thread_pool *pool, thread_pool_task *task) {
    if (pool == NULL || task == NULL) {
        return -1;
    }

    switch (pool->policy) {
        case THREAD_POOL_POLICY_FIFO:
            task->function = pool->queue[pool->tail].function;
            task->argument = pool->queue[pool->tail].argument;

            pool->tail = (pool->tail + 1) % pool->queue_size;
            break;
        case THREAD_POOL_POLICY_SFF:
            thread_pool_task *min_task = heap_extract_min(pool->sff_queue);
            if (min_task == NULL) {
                return -1;
            }
            *task = *min_task;
            free(min_task);  // Free the allocated task
            printf("pick task %d, file_size %d\n", task->conn_fd, task->file_size);
            break;
    }

    pool->task_count--;

    return 0;
}

double get_seconds() {
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (double)t.tv_sec + (double)t.tv_usec / 1e6;
}
