//
// Created by ban on 23/4/25.
//

#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include "pthread.h"

typedef struct task {
    char *key;
    char *value;
} task;

typedef struct task_queue {
    pthread_mutex_t mutex;
    task *tasks; // Array of tasks
    int size; // Number of tasks in the queue
    int capacity; // Capacity of the queue
} task_queue;

int init_task_queue(struct task_queue *q);

void destroy_task_queue(struct task_queue *q);

void enqueue_task(struct task_queue *q, char *key, char *value);

int dequeue_task(struct task_queue *q, char **key, char **value);

#endif //TASK_QUEUE_H
