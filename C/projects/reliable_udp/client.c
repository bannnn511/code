#include "io.c"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 500

int main(int argc, char **argv)
{
  char *host;
  char *message;
  int port;
  int clientfd;


  if (argc != 4)
  {
    fprintf(stderr, "Usage: %s <host> <port> <message>\n", argv[0]);
    exit(1);
  }

  host = argv[1];
  port = atoi(argv[2]);
  if (port <= 0 || port > 65535)
  {
    fprintf(stderr, "Invalid port number: %d\n", port);
    exit(1);
  }

  message = argv[3];
  char res[BUFFSIZE];

  int status = make_request(host, argv[2], message, res);
  if (status < -1)
  {
    perror("make request");
    exit(EXIT_FAILURE);
  }

  return 0;
}
