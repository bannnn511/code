#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <_stdio.h>
#include <_stdlib.h>
#include <__stddef_null.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


#define MAXDATASIZE 100 // max number of bytes we can get at once


void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

int main(const int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: telnet remotehostname port");
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;


    const int status = getaddrinfo(argv[1], argv[2], &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }


    int socket_fd;
    for (p = res; p != NULL; p = p->ai_next) {
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_fd == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_fd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to socket\n");
        exit(EXIT_FAILURE);
    }


    char buf[MAXDATASIZE];
    char ip[INET6_ADDRSTRLEN];
    inet_ntop(p->ai_family, get_in_addr(p->ai_addr), ip, sizeof(ip));
    freeaddrinfo(res);
    printf("connecting to: %s\n", ip);

    int number_bytes = recv(socket_fd, &buf, MAXDATASIZE, 0);
    if (number_bytes == -1) {
        fprintf(stderr, "client: failed to recv\n");
        exit(EXIT_FAILURE);
    }

    buf[number_bytes] = '\0';
    printf("client: received '%s'\n", buf);

    close(socket_fd);

    return 0;
}

