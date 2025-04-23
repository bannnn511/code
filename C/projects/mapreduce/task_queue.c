//
// Created by ban on 23/4/25.
//

#include "task_queue.h"
#include <stdlib.h>
#include "pthread.h"


void init_task_queue(task_queue *q) {
    pthread_mutex_init(&q->mutex, NULL);
    q->size = 0;
    q->capacity = 10;
    q->tasks = malloc(sizeof(task) * q->capacity);
}

void destroy_task_queue(task_queue *q) {
    pthread_mutex_destroy(&q->mutex);
    free(q->tasks);
}

void enqueue_task(task_queue *q, char *key, char *value) {
    pthread_mutex_lock(&q->mutex);
    if (q->size == q->capacity) {
        q->capacity *= 2;
        q->tasks = realloc(q->tasks, sizeof(task) * q->capacity);
    }
    q->tasks[q->size].key = key;
    q->tasks[q->size].value = value;
    q->size++;
    pthread_mutex_unlock(&q->mutex);
}

int dequeue_task(struct task_queue *q, char **key, char **value) {
    pthread_mutex_lock(&q->mutex);
    if (q->size > 0) {
        *key = q->tasks[0].key;
        *value = q->tasks[0].value;
        for (int i = 1; i < q->size; i++) {
            q->tasks[i - 1] = q->tasks[i];
        }
        q->size--;
    } else {
        return -1;
    }

    pthread_mutex_unlock(&q->mutex);

    return 0;
}
