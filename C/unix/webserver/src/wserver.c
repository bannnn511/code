#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include <pthread.h>
#include "thread_helper.h"
#include "threadpool.h"

char default_root[] = ".";

void *connection_handler(void *arg);

//
// ./wserver [-d <basedir>] [-p <portnum>]
//
int main(const int argc, char *argv[]) {
    int c;
    const char *root_dir = default_root;
    int port = 10000;
    int threads = 2;
    int buffersize = 1;
    int policy = THREAD_POOL_POLICY_FIFO;

    while ((c = getopt(argc, argv, "d:p:t:b:s:")) != -1) switch (c) {
            case 'd':
                root_dir = optarg;
                break;
            case 'p':
                port = strtol(optarg, NULL, 10);
                if (port < 0 || port > 65535) {
                    fprintf(stderr, "port number out of range\n");
                    exit(1);
                }
                break;
            case 't':
                threads = strtol(optarg, NULL, 10);
                if (threads <= 0) {
                    fprintf(stderr, "thread count must be positive\n");
                    exit(1);
                }
                break;
            case 'b':
                buffersize = strtol(optarg, NULL, 10);
                if (buffersize <= 0) {
                    fprintf(stderr, "buffer size must be positive\n");
                    exit(1);
                }
                break;
            case 's':
                if (strcmp(optarg, "fifo") == 0) {
                    policy = THREAD_POOL_POLICY_FIFO;
                } else if (strcmp(optarg, "sff") == 0) {
                    policy = THREAD_POOL_POLICY_SFF;
                } else {
                    fprintf(stderr, "unknown policy: %s\n", optarg);
                    exit(1);
                }
                break;
            default:
                fprintf(stderr,
                        "usage: wserver [-d basedir] [-p port] [-t threads] [-b buffersize] [-s "
                        "schedalg]\n");
                exit(1);
        }

    // run out of this directory
    chdir_or_die(root_dir);

    thread_pool *pool = malloc(sizeof(thread_pool));
    if (thread_pool_init(pool, threads, buffersize, policy) != 0) {
        fprintf(stderr, "thread_pool_init failed\n");
        exit(1);
    }

    const int listen_fd = open_listen_fd_or_die(port);
    while (1) {
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        long conn_fd = accept(listen_fd, (sockaddr_t *)&client_addr, (socklen_t *)&client_len);
        if (conn_fd < 0) {
            perror("accept");
            continue;
        }

        // handle client request
        // open
        // 5s

        printf("=== accept connection %ld\n", conn_fd);
        if (thread_pool_add(pool, (void *)connection_handler, (void *)conn_fd) == -1) {
            fprintf(stderr, "fail to add task to worker\n");
            close_or_die(conn_fd);
            break;
        }
    }

    thread_pool_wait(pool);
    thread_pool_destroy(pool);

    return 0;
}

void *connection_handler(void *arg) {
    long conn_fd = (long)arg;
    request_handle(conn_fd);
    printf("=== close connection %ld\n", conn_fd);
    close_or_die(conn_fd);

    return NULL;
}
