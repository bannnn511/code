#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define MYPORT "8000"
#define BACKLOG 20
#define INTERVAL 2 // seconds between requests

volatile sig_atomic_t keep_running = 1;

void sigint_handler(int sig)
{
    keep_running = 0;
}

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main()
{
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    const int status = getaddrinfo(NULL, MYPORT, &hints, &res);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    int socket_fd = -1;
    for (p = res; p != NULL; p = p->ai_next)
    {
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_fd == -1)
        {
            perror("server: socket");
            continue;
        }

        int yes = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        {
            perror("setsockopt");
            close(socket_fd);
            continue;
        }

        // make a socket, bind
        if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(socket_fd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(res);
    if (p == NULL)
    {
        close(socket_fd);
        fprintf(stderr, "server: failed to bind\n");
        exit(EXIT_FAILURE);
    }

    // listen socket
    if (listen(socket_fd, BACKLOG) == -1)
    {
        close(socket_fd);
        fprintf(stderr, "server: failed to listen\n");
        exit(EXIT_FAILURE);
    }

    char hostname[1024];

    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        fprintf(stderr, "Error getting hostname: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("listening on %s port %s\n", hostname, MYPORT);

    // reap child processes
    struct sigaction sa, sa_cancel;
    sigemptyset(&sa.sa_mask);
    sigemptyset(&sa_cancel.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchld_handler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sa_cancel.sa_flags = 0;
    sa_cancel.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &sa_cancel, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    const char *msg = "Hello, Client";
    size_t len = strlen(msg);
    int count = 0;
    while (keep_running)
    {
        // accept incoming connection
        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof(their_addr);
        char buf[INET6_ADDRSTRLEN];
        int newfd = accept(socket_fd, (struct sockaddr *)&their_addr, &addr_size);
        if (newfd == -1)
        {
            // perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), buf, sizeof(buf));
        printf("connection from: %s\n", buf);

        // doc ghi file mat 5s

        pid_t pid;
        switch (pid = fork())
        {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            close(socket_fd);
            pid_t child = getpid();
            while (keep_running)
            {
                char buffer[1024];
                ssize_t number_bytes = recv(newfd, buffer, sizeof(buffer), 0);
                if (number_bytes == 0)
                {
                    printf("server: client disconnect on thread: %d\n", child);
                    break;
                }
                if (number_bytes == -1)
                {
                    fprintf(stderr, "client: failed to recv\n");
                    exit(EXIT_FAILURE);
                }

                size_t bytes_sent = send(newfd, msg, len, 0);
                printf("server: send on thread: %d\n", child);
                if (bytes_sent == -1)
                {
                    perror("server: send");
                }

                printf("bytes sent: %ld\n", bytes_sent);
                sleep(INTERVAL);
            }
            close(newfd);
            exit(0);

        default:
            count++;
        }
        // parent close
        close(newfd);
    }

    close(socket_fd);

    return 0;
}
