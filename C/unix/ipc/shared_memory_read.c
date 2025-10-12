#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHMEM_SIZE 4096

int main(int argc, char *argv[]) {
  int fd;
  size_t len; /* size of shared memory object */
  char *addr;
  struct stat sb;

  if (argc < 2 || strncmp(argv[1], "--help", 6) == 0) {
    fprintf(stderr, "%s shm-name", argv[0]);
    exit(EXIT_FAILURE);
  }

  if ((fd = shm_open(argv[1], O_RDONLY, 0)) == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  if ((fstat(fd, &sb)) == -1) {
    perror("fstat");
    exit(EXIT_FAILURE);
  }

  addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  if (close(fd) == -1) { /* fd is no longer needed */
    perror("close");
    exit(EXIT_FAILURE);
  }

  write(STDOUT_FILENO, addr, sb.st_size);

  shm_unlink(argv[1]);
  return 0;
}
