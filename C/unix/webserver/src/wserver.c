#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include <pthread.h>
#include "thread_helper.h"

char default_root[] = ".";

void *spawn_thread(void *arg);

//
// ./wserver [-d <basedir>] [-p <portnum>]
//
int main(int argc, char *argv[]) {
    int c;
    char *root_dir = default_root;
    int port = 10000;

    while ((c = getopt(argc, argv, "d:p:")) != -1) switch (c) {
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

    // now, get to work
    int listen_fd = open_listen_fd_or_die(port);
    while (1) {
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        int conn_fd =
            accept_or_die(listen_fd, (sockaddr_t *)&client_addr, (socklen_t *)&client_len);

        pthread_t tid;
        thread_create_or_die(&tid, NULL, spawn_thread, (void *)(intptr_t)conn_fd);

        pthread_detach(tid);
    }
    return 0;
}

void *spawn_thread(void *arg) {
    int conn_fd = (int)arg;
    request_handle(conn_fd);
    close_or_die(conn_fd);
}
