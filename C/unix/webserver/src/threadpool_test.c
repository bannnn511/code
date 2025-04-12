#include "threadpool.c"
#include <unistd.h>
#include <sys/time.h>

#include "stdio.h"
#include "stdlib.h"


void hello_world(char *s) {
    printf("hello %s\n", s);
    sleep(2);
}

int main() {
    thread_pool *pool = malloc(sizeof(thread_pool));
    thread_pool_init(pool, 3, 10);

    if (thread_pool_add(pool, (void *) hello_world, "world") == -1) {
        fprintf(stderr, "pool queue full\n");
    }

    if (thread_pool_add(pool, (void *) hello_world, "earth") == -1) {
        fprintf(stderr, "pool queue full\n");
    }

    thread_pool_wait(pool);

    return 0;
}
