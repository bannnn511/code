#include "zemaphore.c"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

zem_t s;

void *child(void *arg) {
  printf("child\n");
  zem_post(&s);
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t p;
  printf("parent: begin\n");
  zem_init(&s, 0);
  Pthread_create(&p, NULL, child, NULL);
  zem_wait(&s);
  printf("parent: end\n");
  return 0;
}
