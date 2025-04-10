#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "socket.c"
#include <fcntl.h>

#define MYPORT "8000"
#define BACKLOG 20
#define INTERVAL 2
#define MAX_SIZE 65536

volatile sig_atomic_t keep_running = 1;

void sigint_handler(int sig) { keep_running = 0; }

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
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
    int nfds = 0;
    if (socket_fd > nfds) {
        nfds = socket_fd;
    }

    struct SocketArray *connected_sockets = create_socket_array(10);
    while (keep_running) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(socket_fd, &read_fds);  // monitoring listening socket

        // Add all connected client sockets
        for (size_t i = 0; i < connected_sockets->size; i++) {
            FD_SET(connected_sockets->sockets[i], &read_fds);
        }

        // Wait for activity on any socket
        printf("server: wait sockets\n");
        const int rc = select(nfds + 1, &read_fds, NULL, NULL, NULL);
        if (rc == -1) {
            if (errno == EINTR) continue;  // Handle interrupt
            perror("server: select");
            break;
        }
        printf("server: socks ready\n");

        // new socket connection from client
        if (FD_ISSET(socket_fd, &read_fds)) {
            printf("server: new connection\n");
            struct sockaddr_storage their_addr;
            socklen_t addr_size = sizeof(their_addr);
            char buf[INET6_ADDRSTRLEN];
            // accept incoming connection
            const int newfd = accept(socket_fd, (struct sockaddr *)&their_addr, &addr_size);
            if (newfd == -1) {
                continue;
            }
            inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), buf,
                      sizeof(buf));
            printf("connection from: %s, domains: %d \n", buf, their_addr.ss_family);
            printf("lists of domains, AFUNIX: %d, IPv4: %d, IPv6:%d\n", AF_UNIX, AF_INET, AF_INET6);

            if (newfd > nfds) {
                nfds = newfd;
            }
            add_socket(connected_sockets, newfd);
            FD_SET(newfd, &read_fds);
        }

        for (size_t i = 0; i < connected_sockets->size; i++) {
            const int sock = connected_sockets->sockets[i];
            if (FD_ISSET(sock, &read_fds)) {
                char buffer[1024];
                const ssize_t number_bytes = recv(sock, buffer, sizeof(buffer), 0);

                if (number_bytes == 0) {
                    printf("server: client disconnect on sock: %d\n", sock);
                    close(sock);
                    remove_socket(connected_sockets, sock);
                    break;
                }
                if (number_bytes == -1) {
                    fprintf(stderr, "server: failed to recv\n");
                    exit(EXIT_FAILURE);
                }

                buffer[number_bytes] = '\0';
                printf("server: received: '%s'\n", buffer);

                // Open the requested file
                printf("server: open and read file\n");
                int fd = open(buffer, O_RDONLY);
                if (fd == -1) {
                    const char *error_msg = "Error: File not found";
                    send(sock, error_msg, strlen(error_msg), 0);
                    printf("server: file '%s' not found\n", buffer);
                } else {
                    // Read and send file in chunks
                    char file_buffer[MAX_SIZE] = {0};
                    size_t total_bytes_sent = 0;
                    size_t bytes_read;

                    while ((bytes_read = read(fd, file_buffer, sizeof(file_buffer))) > 0) {
                        size_t bytes_sent = send(sock, file_buffer, bytes_read, 0);
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
        }
    }

    close(socket_fd);

    return 0;
}
