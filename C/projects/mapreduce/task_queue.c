//
// Created by ban on 23/4/25.
//

#include "task_queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "pthread.h"


int init_task_queue(task_queue *q) {
    if (!q) return -1;
    if (pthread_mutex_init(&q->mutex, NULL) != 0) {
        return -1;
    }

    q->size = 0;
    q->capacity = 10;
    q->tasks = malloc(sizeof(task) * q->capacity);
    if (!q->tasks) {
        pthread_mutex_destroy(&q->mutex);
        return -1;
    }
    return 0;
}

void destroy_task_queue(task_queue *q) {
    pthread_mutex_lock(&q->mutex);
    for (int i = 0; i < q->size; i++) {
        free(q->tasks[i].key);
        free(q->tasks[i].value);
    }
    free(q->tasks);
    pthread_mutex_unlock(&q->mutex);
    pthread_mutex_destroy(&q->mutex);
}

void enqueue_task(task_queue *q, char *key, char *value) {
    pthread_mutex_lock(&q->mutex);
    if (q->size == q->capacity) {
        q->capacity *= 2;
        q->tasks = realloc(q->tasks, sizeof(task) * q->capacity);
    }
    printf("enqueue task: %s\n", key);
    q->tasks[q->size].key = key;
    q->tasks[q->size].value = value;
    q->size++;
    pthread_mutex_unlock(&q->mutex);
}

int dequeue_task(task_queue *q, char **key, char **value) {
    pthread_mutex_lock(&q->mutex);
    if (q->size == 0) {
        *key = NULL;
        *value = NULL;
        pthread_mutex_unlock(&q->mutex);
        return -1;
    }

    *key = q->tasks[0].key;
    *value = q->tasks[0].value;

    for (int i = 1; i < q->size; i++) {
        q->tasks[i - 1] = q->tasks[i];
    }
    q->size--;

    pthread_mutex_unlock(&q->mutex);
    return 0;
}
