#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>

#define MAXDATASIZE 100 // max number of bytes we can get at once
#define INTERVAL 4      // seconds between requests

volatile sig_atomic_t keep_running = 1;

void sigint_handler(int sig) {
    keep_running = 0;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }
    return &((struct sockaddr_in6 *) sa)->sin6_addr;
}

int connect_to_server(const char *hostname, const char *port) {
    struct addrinfo hints, *res, *p;
    int socket_fd;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    char ip[INET6_ADDRSTRLEN];

    for (p = res; p != NULL; p = p->ai_next) {
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_fd == -1) {
            perror("client: socket");
            continue;
        }

        inet_ntop(p->ai_family, get_in_addr(p->ai_addr), ip, sizeof(ip));

        if (connect(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_fd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        freeaddrinfo(res);
        return -1;
    }

    printf("connecting to: %s\n", ip);
    freeaddrinfo(res);

    return socket_fd;
}

int main(const int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s hostname port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Set up signal handler for ctrl+c
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Client started. Press Ctrl+C to stop.\n");

    int request_count = 0;

    int socket_fd = connect_to_server(argv[1], argv[2]);
    while (keep_running) {
        if (socket_fd == -1) {
            sleep(INTERVAL);
            continue;
        }

        request_count++;
        printf("Request #%d\n", request_count);

        // Send a dummy message (empty)
        printf("[%ld] client: sleep\n", time(NULL));
        sleep(1);
        send(socket_fd, "", 0, 0);
        printf("clien: send\n");

        // Receive response
        char buf[MAXDATASIZE];
        ssize_t number_bytes = recv(socket_fd, buf, MAXDATASIZE - 1, 0);
        printf("cliend: recv\n");
        if (number_bytes == -1) {
            perror("client: recv");
        } else if (number_bytes == 0) {
            printf("Server closed connection\n");
            break;
        } else {
            buf[number_bytes] = '\0';
            printf("Received: '%s'\n", buf);
        }

        sleep(INTERVAL);
    }

    printf("\nClient stopped.\n");
    return 0;
}
