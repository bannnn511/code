#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include <liburing.h>
#include <liburing/io_uring.h>

#define QD 2

char default_root[] = ".";

struct request {
    int type;
};

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

    struct io_uring *ring;
    int ret = io_uring_queue_init(QD, ring, 0);
    if (ret < 0) {
        perror("io_uring_queue_init");
        exit(1);
    }
    assert(ring != NULL);

    // now, get to work
    int listen_fd = open_listen_fd_or_die(port);
    while (1) {
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
        if (!sqe) {
            perror("io_uring_get_sqe");
            continue;
        }
        io_uring_prep_accept(sqe, listen_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_len,
                             0);

        struct request *req = malloc(sizeof(struct request));
        io_uring_sqe_set_data(sqe, req);
        io_uring_submit(ring);

        struct io_uring_cqe *cqe;
        ret = io_uring_peek_cqe(ring, &cqe);
        if (ret < 0) {
            perror("io_uring_peek_cqe");
            continue;
        }

        struct io_data *data = io_uring_cqe_get_data(cqe);
        if (cqe->res < 0) {
            fprintf(stderr, "cqe failed: %s\n", strerror(-cqe->res));
            continue;
        }


        io_uring_cqe_seen(ring, cqe);
    }

    return 0;
}
