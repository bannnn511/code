#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include <pthread.h>
#include "thread_helper.h"
#include "threadpool.h"

char default_root[] = ".";

void *spawn_thread_conn(void *arg);

//
// ./wserver [-d <basedir>] [-p <portnum>]
//
int main(int argc, char *argv[]) {
    int c;
    char *root_dir = default_root;
    int port = 10000;

    while ((c = getopt(argc, argv, "d:p:")) != -1)
        switch (c) {
            case 'd':
                root_dir = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "usage: wserver [-d basedir] [-p port]\n");
                exit(1);
        }

    // run out of this directory
    chdir_or_die(root_dir);

    thread_pool *pool = malloc(sizeof(thread_pool));
    thread_pool_init(pool, 2, 100);

    // now, get to work
    const int listen_fd = open_listen_fd_or_die(port);
    while (1) {
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        int conn_fd =
                accept_or_die(listen_fd, (sockaddr_t *)&client_addr, (socklen_t *)&client_len);

        // pthread_t tid;
        // thread_create_or_die(&tid, NULL, spawn_thread_conn, (void *)(intptr_t)conn_fd);
        // pthread_detach(tid);

        if (thread_pool_add(pool, (void *) spawn_thread_conn, (void *) conn_fd) == -1) {
            fprintf(stderr, "fail to add task to worker\n");
            break;
        }
    }

    thread_pool_wait(pool);

    return 0;
}

void *spawn_thread_conn(void *arg) {
    int conn_fd = (int) arg;
    request_handle(conn_fd);
    close_or_die(conn_fd);
}
