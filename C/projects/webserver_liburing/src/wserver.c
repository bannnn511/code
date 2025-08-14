#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include <liburing.h>
#include <liburing/io_uring.h>
#include <execinfo.h>
#include <backtrace.h>


#define QD 2

char default_root[] = ".";

struct request {
    int type;
};

static struct backtrace_state *state = NULL;

static void error_callback(void *data, const char *msg, int errnum) {
    fprintf(stderr, "libbacktrace error: %s (%d)\n", msg, errnum);
}

static void syminfo_callback(void *data, uintptr_t pc,
                             const char *symname, uintptr_t symval, uintptr_t symsize) {
    if (symname)
        fprintf(stderr, "    %s\n", symname);
    else
        fprintf(stderr, "    [no symbol]\n");
}

static int full_callback(void *data, uintptr_t pc, const char *filename, int lineno, const char *function) {
    if (filename && function)
        fprintf(stderr, "%s:%d: %s\n", filename, lineno, function);
    else if (filename)
        fprintf(stderr, "%s:%d\n", filename, lineno);
    else
        fprintf(stderr, "[unknown]\n");
    return 0;
}

void sigsegv_handler(int sig) {
    write(STDERR_FILENO, "Segmentation fault caught!\n", 28);

    void *buffer[100];
    int nptrs = backtrace(buffer, 100);

    for (int i = 0; i < nptrs; i++) {
        uintptr_t addr = (uintptr_t) buffer[i];
        backtrace_syminfo(state, addr, syminfo_callback, error_callback, NULL);
        backtrace_pcinfo(state, addr, full_callback, error_callback, NULL);
    }

    _exit(EXIT_FAILURE);
}


//
// ./wserver [-d <basedir>] [-p <portnum>]
//
int main(int argc, char *argv[]) {
    state = backtrace_create_state("./wserver", 1, error_callback, NULL);

    signal(SIGSEGV, sigsegv_handler);

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

    struct io_uring ring;
    int ret = io_uring_queue_init(QD, &ring, 0);
    if (ret < 0) {
        perror("io_uring_queue_init");
        exit(1);
    }

    // now, get to work
    int listen_fd = open_listen_fd_or_die(port);
    int loops = 0;
    while (true && loops < 1000) {
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
        if (!sqe) {
            perror("io_uring_get_sqe");
            continue;
        }
        io_uring_prep_accept(sqe, listen_fd, (struct sockaddr *) &client_addr,
                             (socklen_t *) &client_len, 0);

        struct request *req = malloc(sizeof(struct request));
        io_uring_sqe_set_data(sqe, req);
        io_uring_submit(&ring);

        struct io_uring_cqe *cqe;
        printf("wait here\n");
        ret = io_uring_wait_cqe(&ring, &cqe);
        printf("wait done %d\n", ret);
        if (ret < 0) {
            loops++;
            continue;
        }

        // struct io_data *data = io_uring_cqe_get_data(cqe);
        if (cqe->res < 0) {
            fprintf(stderr, "cqe failed: %s\n", strerror(-cqe->res));
            continue;
        }

        request_handle(cqe->res);

        io_uring_cqe_seen(&ring, cqe);
        free(req);
    }

    return 0;
}
