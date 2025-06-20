#include <fcntl.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <assert.h>
#include "util.c"

#define TIMEOUT 5000     // Timeout in milliseconds for epoll_wait
#define ACKTIMEOUT 5000  // Timeout for ACK in milliseconds

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
packet_t create_ack_packet(int ack) { return create_packet(ack, "ACK"); }

int UDP_Write(int socket_fd, const struct sockaddr *addr, packet_t *buffer, int n) {
    int addr_len = sizeof(struct sockaddr_in);
    if (strcmp(buffer->buffer, "ACK") == 0)
        print_debug("UDP_Write: sending [ACK = %d]\n", buffer->ack);
    else {
        print_debug("UDP_Write: sending [ACK = %d, message]\n", buffer->ack);
    }
    return sendto(socket_fd, buffer, n, 0, addr, (socklen_t)addr_len);
}

int UDP_Read(int socket_fd, struct sockaddr *addr, packet_t *recvPacket, int n) {
    socklen_t len = sizeof(struct sockaddr_in);
    int bytes_received = recvfrom(socket_fd, recvPacket, n - 1, 0, addr, &len);
    if (bytes_received < 0) {
        perror("UDP_Read: recvfrom failed");
        return -1;
    }

    if (strcmp(recvPacket->buffer, "ACK") == 0) {
        print_debug("UDP_Read: received [ACK = %d]\n", recvPacket->ack);
    } else {
        print_debug("UDP_Read: received [ACK = %d, message]\n", recvPacket->ack);
    }

    return bytes_received;
}

int make_response(int socket_fd, struct sockaddr *addr, char *message, const int ack) {
    int ret = 0;
    assert(ack == 2);
    packet_t packet = create_packet(ack, message);
    packet_t recv_packet;
    memset(&recv_packet, 0, sizeof(recv_packet));
    struct sockaddr_storage their_addr;

    int epfd;
    struct epoll_event ev;

    epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        return -1;
    }

    ev.events = EPOLLIN | EPOLLONESHOT;
    ev.data.fd = socket_fd;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, socket_fd, &ev) == -1) {
        perror("epoll_ctl");
        ret = -1;
        goto cleanup;
    }

    int max_retries = 100;
    int expected_ack = ack;
    assert(expected_ack == 2);

    // ACK = 0: [client] sends a message
    // ACK = 1: [server] acknowledges the message
    // ACK = 2: [server] sends a response
    // ACK = 3: [client] acknowledges the response
    while (max_retries-- > 0) {
        assert(packet.ack == expected_ack);
        // Send the message with ACK = 2
        UDP_Write(socket_fd, addr, &packet, sizeof(packet));
        printf("test");

        setnonblocking(socket_fd);

        int status = epoll_wait(epfd, &ev, 1, TIMEOUT);
        if (status == -1) {
            perror("epoll_wait");
            ret = -1;
            goto cleanup;
        } else if (status == 0) {
            print_debug("No ACK received, retrying...\n");
            packet.ack = expected_ack = ack;  // Reset
            continue;
        }

        expected_ack++;
        int numbytes =
            UDP_Read(socket_fd, (struct sockaddr *)&their_addr, &recv_packet, sizeof(recv_packet));
        if (numbytes == -1) {
            perror("client: recvfrom");
            ret = -1;
            goto cleanup;
        }

        // Wait for ACK = 3
        if (recv_packet.ack != expected_ack) {
            print_debug("ACK mismatch: received %d, expected %d\n", recv_packet.ack, expected_ack);
            print_debug("No valid ACK received, retrying...\n");
            packet.ack = expected_ack = ack;
            continue;
        }

        setblocking(socket_fd);
        break;  // Valid ACK received
    }

    if (max_retries == 0) {
        fprintf(stderr, "Max retries reached, giving up\n");
        ret = -1;
    }

cleanup:
    setblocking(socket_fd);
    close(epfd);

    return ret;
}

int make_request(const char *host, const char *port, const char *message, char *res,
                 size_t res_size) {
    int ret = 0;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int socket_fd = -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // AF_INET or AF_INET6
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        break;
    }

    if (p == NULL || socket_fd == -1) {
        fprintf(stderr, "client: failed to create socket\n");
        free(servinfo);
        return -1;
    }

    const int first_ack = 0;
    packet_t packet = create_packet(first_ack, message);
    packet_t recv_packet;
    int expected_ack = 0;
    memset(&recv_packet, 0, sizeof(recv_packet));
    struct sockaddr_storage their_addr;

    int epfd;
    struct epoll_event ev;
    epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        close(socket_fd);
        return -1;
    }

    ev.events = EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, socket_fd, &ev) == -1) {
        perror("epoll_ctl");
        close(epfd);
        return -1;
    }

    int max_retries = 100;
    while (max_retries-- > 0) {
        // Send the message with ACK = 0
        UDP_Write(socket_fd, p->ai_addr, &packet, sizeof(packet));

        setnonblocking(socket_fd);

        int status = epoll_wait(epfd, &ev, 1, TIMEOUT);
        if (status == -1) {
            perror("epoll_wait");
            ret = -1;
            goto cleanup;
        } else if (status == 0) {
            print_debug("No ACK received, retrying...\n");
            packet.ack = expected_ack = first_ack;  // Resend the same ACK
            continue;
        }

        // Wait for ACK = 1, ACK that the message was received
        expected_ack++;
        int numbytes =
            UDP_Read(socket_fd, (struct sockaddr *)&their_addr, &recv_packet, sizeof(recv_packet));
        if (numbytes == -1) {
            perror("client: recvfrom");
            ret = -1;
            goto cleanup;
        }

        // ACK = 1
        if (expected_ack != recv_packet.ack) {
            print_debug("ACK mismatch: received %d, expected %d\n", recv_packet.ack, expected_ack);
            expected_ack = first_ack;  // Reset expected ACK
            continue;
        }

        // Wait for message
        int status2 = epoll_wait(epfd, &ev, 1, TIMEOUT);
        if (status2 == -1) {
            perror("epoll_wait");
            ret = -1;
            goto cleanup;
        } else if (status == 0) {
            print_debug("No ACK received, retrying...\n");
            packet.ack = expected_ack = first_ack;  // Resend the same ACK
            continue;
        }

        numbytes =
            UDP_Read(socket_fd, (struct sockaddr *)&their_addr, &recv_packet, sizeof(recv_packet));
        if (numbytes == -1) {
            perror("client: recvfrom");
            ret = -1;
            goto cleanup;
        }

        // ACK = 2
        expected_ack++;
        if (recv_packet.ack == expected_ack) {
            // ACK response
            recv_packet.ack = expected_ack + 1;
            packet_t ack = create_ack_packet(recv_packet.ack);
            if (UDP_Write(socket_fd, (struct sockaddr *)&their_addr, &ack, sizeof(ack)) < 0) {
                fprintf(stderr, "send_message: send ACK failed\n");
                ret = -1;
                goto cleanup;
            }
            break;
        } else {
            print_debug("Received unexpected ACK: %d, expected: %d\n", recv_packet.ack,
                        packet.ack + 1);
            print_debug("No valid response received, retrying...\n");
            continue;
        }
    }

    if (max_retries == 0) {
        fprintf(stderr, "Max retries reached, giving up\n");
        ret = -1;
        goto cleanup;
    }

    if (res == NULL) {
        fprintf(stderr, "Response buffer is NULL\n");
        ret = -1;
        goto cleanup;
    }

    size_t copy_len = strnlen(recv_packet.buffer, BUFSIZ);
    if (copy_len >= res_size) {
        copy_len = res_size - 1;
    }
    strncpy(res, recv_packet.buffer, copy_len);
    res[copy_len] = '\0';

cleanup:
    close(socket_fd);
    close(epfd);
    freeaddrinfo(servinfo);

    return ret;
}

typedef void *(*handler)(int socket_fd, struct sockaddr *addr, packet_t *req);

int udp_request_handler(int socket_fd, handler func) {
    while (1) {
        struct sockaddr_in addr;
        packet_t recv_packet;

        int rc = UDP_Read(socket_fd, (struct sockaddr *)&addr, &recv_packet, 500);
        if (rc < 0) {
            fprintf(stderr, "udp_request_handler: recvfrom failed\n");
            return -1;
        }

        // ACK = 0
        assert(recv_packet.ack == 0);

        // ACK = 1
        packet_t ack = create_ack_packet(++recv_packet.ack);
        assert(ack.ack == 1);
        int ack_status = UDP_Write(socket_fd, (struct sockaddr *)&addr, &ack, sizeof(ack));
        if (ack_status < 0) {
            fprintf(stderr, "send_message: send ACK failed\n");
            return -1;
        }

        if (rc > 0) {
            func(socket_fd, (struct sockaddr *)&addr, &recv_packet);
        }

        print_debug("== server handling message done ==\n");
    }
}

int quic_create_server(const char *port) {
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
