#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) ==
        -1) {
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
  int socket_fd;
  int status;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM; // UDP

  status = getaddrinfo(hostname, port, &hints, &res);
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

int UDP_Write(int socket_fd, struct sockaddr *addr, char *buffer, int n) {
  int addr_len = sizeof(struct sockaddr_in);
  return sendto(socket_fd, buffer, n, 0, addr, (socklen_t)addr_len);
}

int UDP_Read(int socket_fd, struct sockaddr *addr, char *buffer, int n) {
  int len = sizeof(struct sockaddr_in);
  return recvfrom(socket_fd, buffer, n, 0, addr, (socklen_t *)&len);
}

int make_request(char *host, char *port, char *req, char *recv) {
  struct addrinfo hints, *res, *p;
  int socket_fd;
  int status;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM; // UDP

  status = getaddrinfo(host, port, &hints, &res);
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

  struct sockaddr_in addrSnd, addrRcv;
  char message[BUFSIZ];
  int rc = UDP_Write(socket_fd, p->ai_addr, req, strlen(req));
  if (rc > 0) {
    rc = UDP_Read(socket_fd, p->ai_addr, message, BUFSIZ);
    printf("client received: %s\n", message);
  } else {
    perror("client: UDP_Write\n");
  }

  return 0;
}

typedef void *(*handler)( int socket_fd, struct sockaddr *addr, char *req);

int udp_request_handler(int socket_fd, handler func) {
  while (1) {
    struct sockaddr_in addr;
    char message[BUFSIZ];

    int rc = UDP_Read(socket_fd, (struct sockaddr *)&addr, message, 500);
    printf("server received: %s\n", message);
    if (rc > 0) {
      char reply[BUFSIZ];
      sprintf(reply, "goodbye world");
      rc = UDP_Write(socket_fd, (struct sockaddr *)&addr, reply,
                     strlen(reply) + 1);
      func(socket_fd, (struct sockaddr *)&addr, message);
    }
  }
  return 0;
}
