#include "io.c"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

void *handle_message(int socket_fd, struct sockaddr *addr, char *req) {
    char reply[BUFSIZ];
    snprintf(reply, sizeof(reply), "Hello %s", req);  // Write to reply buffer

    char resp[BUFSIZ];
    int status = send_message(socket_fd, addr, reply, resp);
    if (status == -1) {
        fprintf(stderr, "send_message timeout\n");
    }

    return NULL;
}

/**
 * Usage: server <port>
 */
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number: %d\n", port);
        exit(1);
    }

    const int socket_fd = udp_create_server(argv[1]);
    if (socket_fd == -1) {
        exit(EXIT_FAILURE);
    }

    udp_request_handler(socket_fd, handle_message);

    return 0;
}
