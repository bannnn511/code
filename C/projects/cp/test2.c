#include <assert.h>
#include <fcntl.h>
#include <liburing.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define QUEUE_DEPTH 1
#define BLOCK_SZ 4096

struct file_info {
  int iov_blocks;
  struct iovec iov[];
};

void output_to_file(int out_fd, char *buf, int len) {
  while (len--) {
    assert(buf != NULL);
    write(out_fd, buf++, 1);
  }
}

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
  assert(blocks > 0);

  struct file_info *fi =
      malloc(sizeof(struct file_info) + blocks * sizeof(struct iovec));
  if (fi == NULL) {
    perror("malloc");
    return 1;
  }

  fi->iov_blocks = blocks;

 