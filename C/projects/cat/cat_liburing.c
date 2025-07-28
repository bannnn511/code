#include <assert.h>
#include <bits/types/struct_iovec.h>
#include <fcntl.h>
#include <liburing.h>
#include <liburing/io_uring.h>
#include <linux/fs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define QUEUE_DEPTH 10
#define BLOCK_SZ 4096

struct file_info {
  int iov_blocks;
  struct iovec iov[];
};

void output_to_console(char *buf, int len) {
  while (len--) {
    assert(buf != NULL);
    fputc(*buf++, stdout);
  }
}

size_t get_file_size(int file) {
  struct stat st;

  if (fstat(file, &st) != 0) {
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

  int size = get_file_size(fd);
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

  size_t bytes_remaining = size;
  int i = 0;
  while (bytes_remaining > 0) {
    size_t this_block = bytes_remaining < BLOCK_SZ ? bytes_remaining : BLOCK_SZ;
    fi->iov[i].iov_base = malloc(this_block);
    if (fi->iov[i].iov_base == NULL) {
      perror("malloc");
      return 1;
    }
    fi->iov[i].iov_len = this_block;
    i++;
    bytes_remaining -= this_block;
  }

  struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
  io_uring_prep_readv(sqe, fd, fi->iov, blocks, 0);
  io_uring_sqe_set_data(sqe, fi);
  io_uring_submit(&ring);

  return 0;
}

int get_completion_and_print(struct io_uring ring) {
  // wait
  struct io_uring_cqe *cqe;
  int ret = io_uring_wait_cqe(&ring, &cqe);
  if (ret < 0) {
    perror("io_uring_wait_cqe");
    return -1;
  }

  if (cqe->res < 0) {
    strerror(abs(cqe->res));
    return -1;
  }

  // get data
  struct file_info *fi = io_uring_cqe_get_data(cqe);
  if (fi == NULL) {
    fprintf(stderr, "cannot get cqe data\n");
    return -1;
  }

  for (int i = 0; i < fi->iov_blocks; i++) {
    output_to_console(fi->iov[i].iov_base, fi->iov[i].iov_len);
  }

  for (int i = 0; i < fi->iov_blocks; i++) {
    free(fi->iov[i].iov_base);
  }
  free(fi);

  io_uring_cqe_seen(&ring, cqe);

  return 0;
}

/**
 * gcc -g -o cat.o cat_liburing.c -luring
 * ulimit -n 1024
 * valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cat.o
 * cat_liburing.c  test.txt
 */
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  struct io_uring ring;
  int rc = io_uring_queue_init(QUEUE_DEPTH, &ring, 0);
  if (rc != 0) {
    perror("io_uring_queue_init");
    return -1;
  }

  for (int i = 1; i < argc; i++) {
    if (submit_read_request(argv[i], ring)) {
      break;
    }

    get_completion_and_print(ring);
  }

  io_uring_queue_exit(&ring);

  return 0;
}
