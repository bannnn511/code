#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


#define MYPORT "8000"
#define BACKLOG 20

void sigchld_handler(int s) {
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

int main() {
    // getaddrinfo
    // socket
    // bind
    // listen
    // accept
    // send

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


    int socket_fd;
    for (p = res; p != NULL; p = p->ai_next) {
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_fd == -1) {
            perror("server: socket");
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

    // reap child processes
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchld_handler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    const char *msg = "Hello, World";
    size_t bytes_sent;
    size_t len = strlen(msg);
    int count = 0;
    while (1) {
        // accept incoming connection
        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof(their_addr);
        char buf[INET6_ADDRSTRLEN];
        int newfd = accept(socket_fd, (struct sockaddr *) &their_addr, &addr_size);
        if (newfd == -1) {
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), buf, sizeof(buf));
        printf("connection from: %s\n", buf);

        if (count > 5) {
            exit(EXIT_SUCCESS);
        }


        pid_t pid;
        switch (pid = fork()) {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0:
                close(socket_fd);
                bytes_sent = send(newfd, msg, len, 0);
                if (bytes_sent == -1) {
                    perror("server: send");
                }
                printf("bytes sent: %ld\n", bytes_sent);
                close(newfd);
                exit(0);
            default:
                count++;
        }
        close(newfd);
    }


    close(socket_fd);


    return 0;
}

