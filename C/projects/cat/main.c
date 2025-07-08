#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/uio.h>

#define BLOCK_SIZE 4096

size_t get_file_size(int fd) {
  struct stat st;
  if (fstat(fd, &st) < 0) {
    perror("fstat");
    return -1;
  }

  if (S_ISREG(st.st_mode)) {
    return st.st_size;
  }

  return 0;
}

void output_console(const char *buf, size_t len) {
  while (len--) {
    fputc(*buf++, stdout);
  }
}

int read_and_print(char *file_name) {
  int fd = open(file_name, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return -1;
  }

  off_t file_size = get_file_size(fd);
  if (file_size < 0) {
    perror("get_file_size");
    return -1;
  }

  int blocks = file_size / BLOCK_SIZE;
  if (file_size % BLOCK_SIZE)
    blocks++;

  struct iovec *iov;
  iov = malloc(blocks * sizeof(struct iovec));
  if (iov == NULL) {
    perror("malloc");
    return -1;
  }

  size_t bytes_remaining = file_size;
  int i = 0;
  while (bytes_remaining > 0) {
    size_t this_block =
        bytes_remaining < BLOCK_SIZE ? bytes_remaining : BLOCK_SIZE;
    iov[i].iov_base = malloc(this_block);
    iov[i].iov_len = this_block;
    i++;
    bytes_remaining -= this_block;
  }

  if (readv(fd, iov, blocks) < 0) {
    perror("readv");
    return -1;
  }

  for (int i = 0; i < blocks; i++) {
    output_console(iov[i].iov_base, iov[i].iov_len);
    free(iov[i].iov_base);
  }

  free(iov);

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    if (read_and_print(argv[i])) {
      fprintf(stderr, "error handling file");
      return -1;
    }
  }

  return 0;
}
