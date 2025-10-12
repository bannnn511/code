#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHMEM_SIZE 4096

int main(int argc, char *argv[]) {
  int fd;
  size_t len; /* size of shared memory object */
  char *addr;

  if (argc < 3 || strncmp(argv[1], "--help", 6) == 0) {
    fprintf(stderr, "%s shm-name string\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if ((fd = shm_open(argv[1], O_RDWR | O_CREAT | O_EXCL, 0666)) == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  len = strlen(argv[2]);
  if (ftruncate(fd, len) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }
  printf("Resized to %ld bytes\n", (long)len);

  addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  if (close(fd) == -1) { /* fd is no longer needed */
    perror("close");
    exit(EXIT_FAILURE);
  }

  printf("copying %ld bytes\n", (long)len);
  memcpy(addr, argv[2], len);

  return 0;
}
