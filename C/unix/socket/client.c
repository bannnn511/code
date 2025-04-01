#include <netdb.h>
#include <_stdio.h>
#include <_stdlib.h>
#include <__stddef_null.h>
#include <string.h>

#define MYPORT "8000"
#define BACKLOG 20

int main() {
    // getaddrinfo
    // socket
    // bind
    // recv

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    printf("getaddrinfo\n");
    int status = getaddrinfo(NULL, MYPORT, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // make a socket, bind and listen
    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(socket_fd, res->ai_addr, res->ai_addrlen);
    listen(socket_fd, BACKLOG);

    // accept incoming connection
    struct sockaddr_storage their_addr;
    int addr_size = sizeof(their_addr);
    int newfd = accept(socket_fd, (struct sockaddr *) &their_addr, &their_addr);


    return 0;
}
