#include <fcntl.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 1000

#define DEBUG 1

typedef struct packet {
    int ack;
    char buffer[BUFSIZ];
} packet_t;

// Helper function to create a packet
packet_t create_packet(int ack, const char *message) {
    packet_t packet;
    packet.ack = ack;
    strncpy(packet.buffer, message, BUFSIZ - 1);
    packet.buffer[BUFSIZ - 1] = '\0';  // Ensure null termination
    return packet;
}

// Helper function to create an ACK packet
packet_t create_ack_packet() { return create_packet(1, "ACK"); }

void print_debug(const char *format, ...) {
    if (!DEBUG) {
        return;
    }
    va_list args;
    va_start(args, format);
    // Get current time
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long milliseconds = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    char time_str[32];
    snprintf(time_str, sizeof(time_str), "%04ld", milliseconds % 10000);
    printf("[%s] ", time_str);
    vprintf(format, args);
    va_end(args);
}

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

int udp_connect_to_host(const char *hostname, const char *port) {
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

int UDP_Write(int socket_fd, const struct sockaddr *addr, const packet_t *buffer, int n) {
    int addr_len = sizeof(struct sockaddr_in);
    return sendto(socket_fd, buffer, n, 0, addr, (socklen_t)addr_len);
}

int UDP_Read(int socket_fd, struct sockaddr *addr, packet_t *recvPacket, int n) {
    socklen_t len = sizeof(struct sockaddr_in);
    int bytes_received = recvfrom(socket_fd, recvPacket->buffer, n - 1, 0, addr,
                                  &len);  // Leave space for null terminator
    if (bytes_received > 0) {
        recvPacket->buffer[bytes_received] = '\0';  // Null-terminate
    }
    return bytes_received;
}

int send_request_timeout(int socket_fd, struct sockaddr *addr, const packet_t *req,
                         packet_t *resp) {
    print_debug("mrt sending: %s\n", req->buffer);
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

    int rc = UDP_Write(socket_fd, addr, req->buffer, strlen(req->buffer));
    if (rc > 0) {
        struct sockaddr_in recv_addr;
        setnonblocking(socket_fd);
        int estatus = epoll_wait(epfd, evlist, MAX_EVENTS, 1000);
        if (estatus == 0) {
            status = -2;
            goto cleanup;
        }

        int bytes_received = UDP_Read(socket_fd, (struct sockaddr *)&recv_addr, resp, BUFSIZ - 1);
        if (bytes_received > 0) {
            resp->buffer[bytes_received] = '\0';
        } else if (bytes_received == 0) {
            print_debug("mrt: no data received\n");
            goto cleanup;
        } else {
            perror("mrt: UDP_Read");
            goto cleanup;
        }
    } else {
        perror("mrt: UDP_Write");
    }

cleanup:
    setblocking(socket_fd);
    close(epfd);

    return status;
}

int wait_and_ack(int socket_fd, struct sockaddr *addr, packet_t *resp) {
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

    setnonblocking(socket_fd);
    print_debug("wait_and_ack: waiting for response\n");
    int rc = epoll_wait(epfd, evlist, MAX_EVENTS, 5000);
    if (rc == 0) {
        print_debug("send_retry: wait for response timeout\n");
        status = -1;
    }

    int bytes_received = UDP_Read(socket_fd, addr, resp, BUFSIZ - 1);
    print_debug("bytes received %d\n", bytes_received);
    if (bytes_received == 0) {
        print_debug("send_retry: no data received\n");
        status = -1;
        goto cleanup;
    } else if (bytes_received < 0) {
        perror("send_retry: UDP_Read");
        status = -1;
        goto cleanup;
    }

cleanup:
    setblocking(socket_fd);
    close(epfd);

    return status;
}

int send_message_ack(int socket_fd, struct sockaddr *addr, const packet_t *req) {
    // send message and wait for ACK
    // if no ACK or timeout -> retry
    int retry = 0;
    // int delay = 0;
    const int MAX_RETRIES = 5;  // Maximum number of retries

    while (retry <= MAX_RETRIES) {
        if (retry > 0) {
            print_debug("send_message: retry sending message %d\n", retry);
            // sleep(delay);
        }

        packet_t ack_packet;
        strcpy(ack_packet.buffer, "ACK");
        int status = send_request_timeout(socket_fd, addr, req, &ack_packet);

        if (status < 0) {
            if (status == -2) {  // Timeout
                print_debug("send_message: timeout on attempt %d\n", retry + 1);
                retry++;
                continue;
            } else {
                print_debug("send_message: error on attempt %d\n", retry + 1);
                return -1;
            }
        }

        if (strncmp("ACK", ack_packet.buffer, 3) != 0) {
            print_debug("send_message ERROR: expected ACK, got: %s\n", ack_packet.buffer);
            retry++;
            continue;
        } else {
            print_debug("send_message: received ACK\n");
            return 0;
        }
    }

    print_debug("send_message: max retries (%d) exceeded\n", MAX_RETRIES);
    return -1;
}

int send_message(int socket_fd, struct sockaddr *addr, char *req) {
    packet_t req_packet;
    strcpy(req_packet.buffer, req);

    return send_message_ack(socket_fd, addr, &req_packet);
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

    packet_t req_ack;
    req_ack.ack = 0;
    strcpy(req_ack.buffer, req);

    print_debug("client: step1\n");
    int send_status = send_message_ack(socket_fd, p->ai_addr, &req_ack);
    if (send_status == -1) {
        print_debug("send_message failed\n");
    }

    print_debug("client: step2\n");
    packet_t recv_packet;
    int wait = wait_and_ack(socket_fd, p->ai_addr, &recv_packet);
    if (wait < 0) {
        fprintf(stderr, "send_message: wait for response failed\n");
        return -1;
    }
    strcpy(recv, recv_packet.buffer);

    print_debug("client: step3\n");
    packet_t ack;
    strcpy(ack.buffer, "ACK");
    int ack_status = send_message_ack(socket_fd, p->ai_addr, &ack);
    if (ack_status < 0) {
        fprintf(stderr, "send_message: send ACK failed\n");
        return -1;
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

        packet_t recv_packet;
        int rc = UDP_Read(socket_fd, (struct sockaddr *)&addr, &recv_packet, 500);
        print_debug("server: received: %s\n", recv_packet.buffer);

        packet_t ack;
        strcpy(ack.buffer, "ACK");
        int ack_status = send_message_ack(socket_fd, (struct sockaddr *)&addr, &ack);
        if (ack_status < 0) {
            fprintf(stderr, "send_message: send ACK failed\n");
            return -1;
        }

        if (rc > 0) {
            func(socket_fd, (struct sockaddr *)&addr, message);
        }

        print_debug("== server handling message done ==\n");
    }
}
