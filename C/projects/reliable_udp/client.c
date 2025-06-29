#include "quic.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 500

/**
 * Usage: client <host> <port> <message>
 */
int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <host> <port> <message>\n", argv[0]);
        exit(1);
    }

    const char *host = argv[1];
    const int port = atoi(argv[2]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number: %d\n", port);
        exit(1);
    }

    char *message = argv[3];
    char res[BUFFSIZE];
    memset(res, 0, sizeof(res));

    int status = make_request(host, argv[2], message, res, sizeof(res));
    if (status < 0) {
        perror("make request");
        exit(EXIT_FAILURE);
    } else {
        printf("Response: %s", res);
    }

    return 0;
}
