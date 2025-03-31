#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 500

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: showip hostname\n");
    exit(EXIT_FAILURE);
  }

  struct addrinfo hints;
  struct addrinfo *servinfo, *p;
  char ipstr[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  int status = getaddrinfo(argv[1], NULL, &hints, &servinfo);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    exit(EXIT_FAILURE);
  }

  printf("IP addresses for %s:\n", argv[1]);

  for (p = servinfo; p != NULL; p = p->ai_next) {
    void *addr;

    // Get the pointer to the address itself
    if (p->ai_family == AF_INET) {
      // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
      addr = &(ipv4->sin_addr);
      inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
      printf("IPv4: %s\n", ipstr);
    } else if (p->ai_family == AF_INET6) {
      // IPv6
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
      addr = &(ipv6->sin6_addr);
      inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
      printf("IPv6: %s\n", ipstr);
    }
  }

  freeaddrinfo(servinfo); // Free the linked list

  return 0;
}
