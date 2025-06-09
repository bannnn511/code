#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MYPORT "8000"
#define BACKLOG 20
#define INTERVAL 2
#define MAX_SIZE 65536
#define MAX_EVENTS 1000

volatile sig_atomic_t keep_running = 1;

void sigint_handler(int sig) { keep_running = 0; }

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void setnonblocking(int fd) {
    int val;
    if ((val = fcntl(fd, F_GETFL), 0) < 0) {
        perror("fcntl");
    }

    val |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, val) < 0) {
        perror("fcntl");
    }
}

int create_server() {
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    const int status = getaddrinfo(NULL, MYPORT, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    int socket_fd = -1;
    for (p = res; p != NULL; p = p->ai_next) {
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_fd == -1) {
            perror("server: socket");
            continue;
        }

        int yes = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("setsockopt");
            close(socket_fd);
            continue;
        }

        // make a socket, bind
        if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_fd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(res);
    if (p == NULL) {
        close(socket_fd);
        fprintf(stderr, "server: failed to bind\n");
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

const char *error_msg = "Error: File not found";

void handle_request(int fd) {
    char buffer[1024];
    const ssize_t number_of_bytes = recv(fd, buffer, sizeof(buffer), 0);
    if (number_of_bytes == 0) {
        printf("server: disconnect on sock: %d\n", fd);
        close(fd);
    } else if (number_of_bytes == -1) {
        fprintf(stderr, "server: failed to recv\n");
        exit(EXIT_FAILURE);
    }

    buffer[number_of_bytes] = '\0';
    printf("server: received: '%s'\n", buffer);
    printf("server: open and read file\n");
    int file_fd = open(buffer, O_RDONLY);
    if (file_fd == -1) {
        send(fd, error_msg, strlen(error_msg), 0);
    } else {
        char file_buffer[MAX_SIZE] = {0};
        size_t total_bytes_sent = 0;
        size_t bytes_read;

        while ((bytes_read = read(file_fd, file_buffer, sizeof(file_buffer))) > 0) {
            size_t bytes_sent = send(fd, file_buffer, bytes_read, 0);
            if (bytes_sent == -1) {
                perror("server: send file chunk");
                close(fd);
                break;
            }
            total_bytes_sent += bytes_sent;
        }

        printf("server: sent file content, %ld bytes total\n", total_bytes_sent);
    }
}

int main() {
    const int socket_fd = create_server();

    // listen socket
    if (listen(socket_fd, BACKLOG) == -1) {
        close(socket_fd);
        fprintf(stderr, "server: failed to listen\n");
        exit(EXIT_FAILURE);
    }

    char hostname[1024];

    if (gethostname(hostname, sizeof(hostname)) != 0) {
        fprintf(stderr, "Error getting hostname: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("listening on %s port %s\n", hostname, MYPORT);

    struct sigaction sa_cancel;
    sigemptyset(&sa_cancel.sa_mask);
    sa_cancel.sa_flags = 0;
    sa_cancel.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &sa_cancel, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    const char *msg = "Hello, Client";
    const size_t len = strlen(msg);

    int epfd, nfds, conn_soc;
    struct epoll_event ev;
    struct epoll_event evlist[MAX_EVENTS];

    epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;  // use level-triggered for socket
    ev.data.fd = socket_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, socket_fd, &ev) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    while (keep_running) {
        nfds = epoll_wait(epfd, evlist, MAX_EVENTS, -1);  // block until an event occur
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        struct epoll_event p;

        for (int i = 0; i < nfds; i++) {
            if (evlist[i].data.fd == socket_fd) {
                // socket receives a new connection
                struct sockaddr_storage their_addr;
                socklen_t addr_size = sizeof their_addr;
                conn_soc = accept(socket_fd, (struct sockaddr *)&their_addr, &addr_size);
                if (conn_soc == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                setnonblocking(conn_soc);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_soc;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, conn_soc, &ev) == -1) {
                    printf("here\n");
                    perror("epoll_ctl");
                    exit(EXIT_FAILURE);
                }
            } else {
                handle_request(evlist[i].data.fd);
            }
        }
    }

    close(socket_fd);

    return 0;
}
