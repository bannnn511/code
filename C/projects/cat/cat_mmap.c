#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return 1;
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    perror("open");
    return -1;
  }

  struct stat st;
  if (fstat(fd, &st) == -1) {
    perror("fstat");
    return -1;
  }

  char *addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (addr == MAP_FAILED) {
    perror("mmap");
    return -1;
  }

  if (write(STDOUT_FILENO, addr, st.st_size) == st.st_size) {
    perror("write");
    return -1;
  }

  if (munmap(addr, st.st_size) == -1) {
    perror("munmap");
    return -1;
  }

  return 0;
}
