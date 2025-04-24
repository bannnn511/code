#include "threadpool.c"
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Simple task function that sleeps for a specified time
void sleep_task(void *arg) {
    int seconds = *((int *) arg);
    // printf("Task sleeping for %d seconds\n", seconds);
    sleep(seconds);
    // printf("Task woke up after %d seconds\n", seconds);
}

// Task that increments a counter with mutex protection
void increment_task(void *arg) {
    int *counter = (int *) arg;
    pthread_mutex_t *mutex = (pthread_mutex_t *) (counter + 1);

    pthread_mutex_lock(mutex);
    (*counter)++;
    pthread_mutex_unlock(mutex);

    // Add some variable workload
    usleep(rand() % 100000); // 0-100ms
}

// Test basic functionality with multiple tasks
void test_basic_functionality() {
    printf("\n=== Test: Basic Functionality ===\n");
    thread_pool *pool = malloc(sizeof(thread_pool));
    assert(thread_pool_init(pool, 2, 5) == 0);

    int sleep1 = 1, sleep2 = 2, sleep3 = 1;

    thread_pool_add(pool, (void *) sleep_task, &sleep1);
    thread_pool_add(pool, (void *) sleep_task, &sleep2);
    thread_pool_add(pool, (void *) sleep_task, &sleep3);

    thread_pool_wait(pool);
    sleep(2);
    thread_pool_destroy(pool);
    free(pool);
    printf("Basic functionality test passed\n");
}

// Test the thread pool with a full queue
void test_full_queue() {
    printf("\n=== Test: Full Queue ===\n");
    thread_pool *pool = malloc(sizeof(thread_pool));
    assert(thread_pool_init(pool, 1, 3) == 0);

    int sleep_times[5] = {1, 1, 1, 1, 1};

    // Add tasks to fill the queue
    for (int i = 0; i < 3; i++) {
        assert(thread_pool_add(pool, (void *)sleep_task, &sleep_times[i]) == 0);
    }

    // This should block until a task completes
    printf("Adding task after queue is full (should block)...\n");
    thread_pool_add(pool, (void *) sleep_task, &sleep_times[3]);
    printf("Task added successfully after space became available\n");

    thread_pool_wait(pool);
    thread_pool_destroy(pool);
    free(pool);
    printf("Full queue test passed\n");
}

// Test high concurrency with many tasks and threads
void test_high_concurrency() {
    printf("\n=== Test: High Concurrency ===\n");

    const int NUM_THREADS = 8;
    const int NUM_TASKS = 1000;

    thread_pool *pool = malloc(sizeof(thread_pool));
    assert(thread_pool_init(pool, NUM_THREADS, 32) == 0);

    // Setup a shared counter with mutex
    int *shared_data = malloc(sizeof(int) + sizeof(pthread_mutex_t));
    *shared_data = 0; // Counter starts at 0
    pthread_mutex_t *mutex = (pthread_mutex_t *) (shared_data + 1);
    pthread_mutex_init(mutex, NULL);

    // Add many tasks that increment the counter
    for (int i = 0; i < NUM_TASKS; i++) {
        thread_pool_add(pool, increment_task, shared_data);
    }

    thread_pool_wait(pool);

    // Verify the counter
    printf("Counter value: %d (expected %d)\n", *shared_data, NUM_TASKS);
    assert(*shared_data == NUM_TASKS);

    pthread_mutex_destroy(mutex);
    free(shared_data);
    thread_pool_destroy(pool);
    free(pool);
    printf("High concurrency test passed\n");
}

// Test the shutdown behavior
void test_shutdown() {
    printf("\n=== Test: Shutdown Behavior ===\n");
    thread_pool *pool = malloc(sizeof(thread_pool));
    assert(thread_pool_init(pool, 4, 8) == 0);

    int sleep_long = 3;
    int sleep_short = 1;

    // Add some long-running tasks
    for (int i = 0; i < 3; i++) {
        thread_pool_add(pool, (void *) sleep_task, &sleep_long);
    }

    // Add some quick tasks
    for (int i = 0; i < 5; i++) {
        thread_pool_add(pool, (void *) sleep_task, &sleep_short);
    }

    // Don't wait, just destroy (should still complete all tasks)
    printf("Destroying pool with tasks still running...\n");
    thread_pool_destroy(pool);
    free(pool);
    printf("Shutdown test passed\n");
}


int main() {
    // Seed random number generator
    srand(time(NULL));

    test_basic_functionality();
    test_full_queue();
    test_high_concurrency();
    test_shutdown();

    printf("\nAll thread pool tests completed successfully!\n");
    return 0;
}
