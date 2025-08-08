#include "cp_liburing_2.h"
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

#define QUEUE_DEPTH 1
#define BLOCK_SZ 1024

struct io_data {
  int offset;
  int size;
  struct iovec iov;
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

/**
 * copy_files run 2 loops
 * first loop, prepare as much read requests as possible
 * second loop, wait for completion and write to output file
 */
int copy_files(struct io_uring ring, int input_fd, int output_fd) {
  size_t size = get_file_size(input_fd);
  struct io_uring_sqe *sqe;

  int offset = 0;
  int loops = 0;
  while (size) {
    size_t this_size = size < BLOCK_SZ ? size : BLOCK_SZ;
    assert(this_size > 0);

    sqe = io_uring_get_sqe(&ring);
    if (!sqe) {
      return 1;
    }
    struct io_data *data = malloc(sizeof(struct io_data) + this_size);
    if (!data) {
      return 1;
    }
    data->iov.iov_base = data + 1;
    data->iov.iov_len = this_size;
    data->offset = offset;
    // data->size = size;
    printf("read: data offset %d\n", data->offset);

    io_uring_prep_readv(sqe, input_fd, &data->iov, 1, offset);
    io_uring_sqe_set_data(sqe, data);
    io_uring_submit(&ring);

    size -= this_size;
    offset += this_size;
    loops++;
  }

  // printf("=== loops : %d\n", loops);

  int write_loops = 0;
  while (write_loops < loops) {
    struct io_uring_cqe *cqe;
    int ret = io_uring_wait_cqe(&ring, &cqe);
    if (ret < 0) {
      fprintf(stderr, "io_uring_wait_cqe: %s\n", strerror(-ret));
      return 1;
    }

    struct io_data *data = io_uring_cqe_get_data(cqe);
    if (cqe->res < 0) {
      fprintf(stderr, "cqe failed: %s\n", strerror(-cqe->res));
      return 1;
    }

    printf("write: data offset %d\n", data->offset);

    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_writev(sqe, output_fd, &data->iov, 1, data->offset);
    io_uring_sqe_set_data(sqe, data);
    int submit_ret = io_uring_submit(&ring);
    if (submit_ret < 0) {
      fprintf(stderr, "submit write failed: %s\n", strerror(-submit_ret));
      return 1;
    }
    io_uring_cqe_seen(&ring, cqe);

    write_loops++;
  }
  printf("=== write loops : %d\n", write_loops);

  // Third loop: wait for all writes to complete and free memory
  for (int i = 0; i < loops; i++) {
    struct io_uring_cqe *cqe;
    int ret = io_uring_wait_cqe(&ring, &cqe);
    if (ret < 0) {
      fprintf(stderr, "io_uring_wait_cqe write: %s\n", strerror(-ret));
      return 1;
    }

    struct io_data *data = io_uring_cqe_get_data(cqe);
    if (cqe->res < 0) {
      fprintf(stderr, "write cqe failed: %s\n", strerror(-cqe->res));
      return 1;
    }

    printf("write completed: offset %d, bytes %d\n", data->offset, cqe->res);

    free(data); // Free the allocated memory
    io_uring_cqe_seen(&ring, cqe);
  }

  return 0;
}

/**
 * gcc -g -o cp.o cp_liburing.c -luring
 * valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cp.o
 * cp_liburing.c test2
 */
int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage: %s <filename> <destination>\n", argv[0]);
    return 1;
  }

  int in_fd = open(argv[1], O_RDONLY);
  if (in_fd < 0) {
    perror("open");
    return -1;
  }

  int out_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (out_fd < 0) {
    perror("open");
    return -1;
  }

  struct io_uring ring;
  int rc = io_uring_queue_init(QUEUE_DEPTH, &ring, 0);
  if (rc < 0) {
    perror("io_uring_queue_init");
    return 1;
  }
  copy_files(ring, in_fd, out_fd);

  io_uring_queue_exit(&ring);

  return 0;
}
