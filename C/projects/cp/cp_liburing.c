#include <fcntl.h>
#include <liburing.h>
#include <stdio.h>
#include <sys/stat.h>

#define QUEUE_DEPTH 1
#define BLOCK_SZ 4096

size_t get_file_size(int fd) {
  struct stat st;
  if (fstat(fd, &st) < 0) {
    perror("fstat");
    return -1;
  }

  return st.st_size;
}

int submit_read_request(char *file_name, struct io_uring ring) {
  int fd = open(file_name, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }

  size_t size = get_file_size(fd);
  int blocks = size / BLOCK_SZ;
  if (size % BLOCK_SZ) {
    blocks++;
  }

  return 1;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage: %s <filename> <destination>\n", argv[0]);
    return 1;
  }

  struct io_uring ring;
  int rc = io_uring_queue_init(QUEUE_DEPTH, &ring, 0);
  if (rc < 0) {
    perror("io_uring_queue_init");
    return 1;
  }

  if (submit_read_request(argv[1], ring)) {
    fprintf(stderr, "cannot submit read request to iouring");
    return 1;
  }

  io_uring_queue_exit(&ring);

  return 0;
}
