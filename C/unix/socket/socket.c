#include <sys/socket.h>
#include <netdb.h>
#include <_stdio.h>
#include <_stdlib.h>
#include <string.h>

int main() {
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    int status = getaddrinfo("www.example.com", "3049", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (connect(socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    return 0;
}
