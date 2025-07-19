#include <bits/types/struct_iovec.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/uio.h>

#define BLOCK_SIZE 4096

int get_file_size(int fd) {
  struct stat st;
  if (fstat(fd, &st) < 0) {
    perror("fstat");
    return -1;
  }

  return st.st_size;
}

int read_and_copy(int fd, int destination) {
  off_t file_size = get_file_size(fd);
  if (file_size < 0) {
    perror("get_file_size");
    return -1;
  }

  int blocks = file_size / BLOCK_SIZE;
  if (file_size % BLOCK_SIZE) {
    blocks++;
  }

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

  if (writev(destination, iov, blocks) < 0) {
    perror("writev");
    return -1;
  }

  for (int i = 0; i < blocks; i++) {
    free(iov[i].iov_base);
  }

  free(iov);

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage: %s <filename> <destination>\n", argv[0]);
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  int fd2 = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd2 < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  read_and_copy(fd, fd2);

  return 0;
}