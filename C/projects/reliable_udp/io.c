#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 1000

void setnonblocking(int fd) {
    int val;
    if ((val = fcntl(fd, F_GETFL), 0) < 0) {
        perror("fcntl");
    }

    val |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, val) < 0) {
        perror("fcntl");
    }
}

void setblocking(int fd) {
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        perror("fcntl");
        return;
    }

    val &= ~O_NONBLOCK;  // Clear the O_NONBLOCK flag

    if (fcntl(fd, F_SETFL, val) < 0) {
        perror("fcntl");
    }
}

int udp_create_server(const char *port) {
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_DGRAM;

    const int status = getaddrinfo(NULL, port, &hints, &res);
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

int udp_connet_to_host(const char *hostname, const char *port) {
    struct addrinfo hints, *res, *p;
    int socket_fd = 0;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;  // UDP

    int status = getaddrinfo(hostname, port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_fd == -1) {
            perror("client: socket");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        freeaddrinfo(res);
        return -1;
    }

    freeaddrinfo(res);

    return socket_fd;
}

int UDP_Write(int socket_fd, const struct sockaddr *addr, const char *buffer, int n) {
    int addr_len = sizeof(struct sockaddr_in);
    return sendto(socket_fd, buffer, n, 0, addr, (socklen_t)addr_len);
}

int UDP_Read(int socket_fd, struct sockaddr *addr, char *buffer, int n) {
    socklen_t len = sizeof(struct sockaddr_in);
    int bytes_received =
        recvfrom(socket_fd, buffer, n - 1, 0, addr, &len);  // Leave space for null terminator
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';  // Null-terminate
    }
    return bytes_received;
}

int send_message(int socket_fd, struct sockaddr *addr, const char *req, char *resp) {
    int status = 0;
    struct epoll_event ev;
    struct epoll_event evlist[MAX_EVENTS];

    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create");
        return -1;
    }
    ev.events = EPOLLIN | EPOLLONESHOT;
    ev.data.fd = socket_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, socket_fd, &ev) == -1) {
        perror("epoll_ctl");
        return -1;
    }

    int rc = UDP_Write(socket_fd, addr, req, strlen(req));
    if (rc > 0) {
        struct sockaddr_in recv_addr;
        setnonblocking(socket_fd);
        int status = epoll_wait(epfd, evlist, MAX_EVENTS, 1000);
        if (status == 0) {
            printf("request timeout\n");
            status = -1;
            goto cleanup;
        }
        int bytes_received = UDP_Read(socket_fd, (struct sockaddr *)&recv_addr, resp, BUFSIZ - 1);

        if (bytes_received > 0) {
            resp[bytes_received] = '\0';  // Null-terminate the string
        } else if (bytes_received == 0) {
            printf("client: no data received\n");
        } else {
            perror("client: UDP_Read");
        }

        char ack_message[] = "ACK";
        char ack_len = strlen(ack_message);
        int rc2 = UDP_Write(socket_fd, addr, ack_message, ack_len);
        if (rc2 == -1) {
            perror("UDP_Write: ACK failed");
            status = -2;
            goto cleanup;
        }
    } else {
        perror("client: UDP_Write");
    }

cleanup:
    setblocking(socket_fd);

    return 0;
}

int make_request(const char *host, const char *port, const char *req, char *recv) {
    struct addrinfo hints, *res, *p;
    int socket_fd = -1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;  // UDP

    int status = getaddrinfo(host, port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_fd == -1) {
            perror("client: socket");
            continue;
        }
        break;
    }

    if (p == NULL || socket_fd == -1) {
        fprintf(stderr, "client: failed to connect\n");
        freeaddrinfo(res);
        close(socket_fd);
        return -1;
    }

    int send_status = send_message(socket_fd, p->ai_addr, req, recv);
    if (send_status == -1) {
        printf("send_message failed\n");
    }

    freeaddrinfo(res);
    close(socket_fd);

    return 0;
}

typedef void *(*handler)(int socket_fd, struct sockaddr *addr, char *req);

int udp_request_handler(int socket_fd, handler func) {
    while (1) {
        struct sockaddr_in addr;
        char message[BUFSIZ];

        int rc = UDP_Read(socket_fd, (struct sockaddr *)&addr, message, 500);
        printf("server received: %s\n", message);
        if (rc > 0) {
            func(socket_fd, (struct sockaddr *)&addr, message);
        }
    }
}
