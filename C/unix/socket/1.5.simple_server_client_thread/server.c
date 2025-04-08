#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MYPORT "8000"
#define BACKLOG 20
#define INTERVAL 2

volatile sig_atomic_t keep_running = 1;

struct client_data {
    int socket_fd;
    struct sockaddr_storage addr;
};

void sigint_handler(int sig) {
    keep_running = 0;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

void *handle_client(void *arg) {
    struct client_data *data = (struct client_data *) arg;
    const int client_socket = data->socket_fd;
    char client_ip[INET6_ADDRSTRLEN];

    inet_ntop(data->addr.ss_family,
              get_in_addr((struct sockaddr *) &data->addr),
              client_ip, sizeof(client_ip));

    printf("server: new thread handling client: %s\n", client_ip);

    const char *msg = "Hello, Client";
    const size_t len = strlen(msg);

    if (send(client_socket, msg, len, 0) == -1) {
        perror("server: send");
    }

    printf("server: sent response to %s\n", client_ip);

    close(client_socket);
    free(data);

    return NULL;
}

int main() {
    struct addrinfo hints = {0}, *res, *p;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(NULL, MYPORT, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return EXIT_FAILURE;
    }

    int socket_fd = -1;
    for (p = res; p != NULL; p = p->ai_next) {
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_fd == -1) continue;

        int yes = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            close(socket_fd);
            continue;
        }

        if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_fd);
            continue;
        }
        break;
    }

    freeaddrinfo(res);
    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        return EXIT_FAILURE;
    }

    if (listen(socket_fd, BACKLOG) == -1) {
        perror("listen");
        return EXIT_FAILURE;
    }

    struct sigaction sa = {
        .sa_handler = sigint_handler,
        .sa_flags = 0
    };
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("server: listening on port %s\n", MYPORT);

    while (keep_running) {
        struct client_data *client = malloc(sizeof(struct client_data));
        socklen_t addr_size = sizeof(client->addr);

        // accept new connection here
        client->socket_fd = accept(socket_fd,
                                   (struct sockaddr *) &client->addr,
                                   &addr_size);

        if (client->socket_fd == -1) {
            free(client);
            continue;
        }

        // create new thread to handle new connection
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, client) != 0) {
            perror("pthread_create");
            free(client);
            close(client->socket_fd);
            continue;
        }
        pthread_detach(thread);
    }

    close(socket_fd);
    printf("\nserver: shutting down\n");
    return 0;
}
