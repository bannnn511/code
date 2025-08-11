#include "cp_liburing_2.h"
#include <assert.h>
#include <bits/types/struct_iovec.h>
#include <fcntl.h>
#include <liburing.h>
#include <liburing/io_uring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define QUEUE_DEPTH 2
#define BLOCK_SZ (16 * 1024)

struct io_data {
  int read;
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

int queue_read(struct io_uring *ring, int fd, size_t size, size_t offset) {
  struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
  if (!sqe) {
    return 1;
  }

  struct io_data *data = malloc(sizeof(struct io_data) + size);
  if (!data) {
    return -1;
  }

  data->iov.iov_base = data + 1;
  data->iov.iov_len = size;
  data->offset = offset;
  data->size = size;
  data->read = 1;

  io_uring_prep_readv(sqe, fd, &data->iov, 1, data->offset);
  io_uring_sqe_set_data(sqe, data);

  return 0;
}

int queue_write(struct io_uring *ring, struct io_uring_sqe *sqe, int fd, struct io_data *data) {
  if (!sqe || !data) {
    return -1;
  }

  data->read = 0;

  io_uring_prep_writev(sqe, fd, &data->iov, 1, data->offset);
  io_uring_sqe_set_data(sqe, data);
  io_uring_submit(ring);
  return 0;
}

/**
 * copy_files run 2 loops
 * first loop, prepare as much read requests as possible
 * second loop, wait for completion and write to output file
 */
int copy_files(struct io_uring ring, int input_fd, int output_fd) {
  size_t size = get_file_size(input_fd);
  struct io_uring_sqe *sqe;

  int write_left = size;
  int offset = 0;
  while (size) {
    size_t this_size = size < BLOCK_SZ ? size : BLOCK_SZ;
    assert(this_size > 0);

    queue_read(&ring, input_fd, this_size, offset);

    io_uring_submit(&ring);

    size -= this_size;
    offset += this_size;
  }

  int ret = io_uring_submit(&ring);
  if (ret < 0) {
    fprintf(stderr, "io_uring_submit: %s\n", strerror(-ret));
    return 1;
  }

  int writes = 0;
  while (write_left) {
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

    sqe = io_uring_get_sqe(&ring);
    queue_write(&ring, sqe, output_fd, data);

    io_uring_cqe_seen(&ring, cqe);

    write_left -= data->size;
    writes++;
  }

  // Third loop: wait for all writes to complete and free memory
  for (int i = 0; i < writes; i++) {
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

    free(data); // Free the allocated memory
    io_uring_cqe_seen(&ring, cqe);
  }

  return 0;
}

int copy_files2(struct io_uring ring, int input_fd, int output_fd) {
  size_t size = get_file_size(input_fd);
  struct io_uring_sqe *sqe;

  int write_left = size;
  size_t offset = 0;
  int reads = 0, writes = 0;
  while (size) {
    int had_reads = reads;
    int completion = 0;
    size_t this_size = size < BLOCK_SZ ? size : BLOCK_SZ;
    assert(this_size > 0);

    while (size) {
      if (reads + writes >= QUEUE_DEPTH) {
        break;
      }

      if (queue_read(&ring, input_fd, this_size, offset)) {
        break;
      }


      size -= this_size;
      offset += this_size;
      reads++;
    }

    if (had_reads != reads) {
      int ret = io_uring_submit(&ring);
      if (ret < 0) {
        fprintf(stderr, "io_uring_submit: %s\n", strerror(-ret));
        break;
      }
    }


    completion = 0;
    while (write_left) {
      struct io_uring_cqe *cqe;
      if (!completion) {
        int ret = io_uring_wait_cqe(&ring, &cqe);
        if (ret < 0) {
          fprintf(stderr, "io_uring_wait_cqe: %s\n", strerror(-ret));
          return 1;
        }

        completion = 1;
      } else {
        int ret = io_uring_peek_cqe(&ring, &cqe);
        if (ret < 0) {
          if (ret == -EAGAIN) {
            io_uring_cqe_seen(&ring, cqe);
            break;
          }
          fprintf(stderr, "io_uring_wait_cqe: %s\n", strerror(-ret));
          return 1;
        }
      }

      if (!cqe) {
        break;
      }

      struct io_data *data = io_uring_cqe_get_data(cqe);
      if (cqe->res < 0) {
        fprintf(stderr, "cqe failed: %s\n", strerror(-cqe->res));
        return 1;
      }


      if (data->read) {
        sqe = io_uring_get_sqe(&ring);
        queue_write(&ring, sqe, output_fd, data);
        write_left -= data->size;
        writes++;
        reads--;
      } else {
        free(data);
        writes--;
      }

      io_uring_cqe_seen(&ring, cqe);
    }
  }

  return 0;
}

/**
 * gcc -g -o cp.o cp_liburing.c -luring
 * valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cp.o
 * cp_liburing.c test2
 *
 * copy_file
 * cmd: time ./cp.o big-file test
 * cqe failed: Invalid argument
 * ________________________________________________________
 * Executed in   43.17 secs    fish           external
 *    usr time    1.28 secs    0.49 millis    1.28 secs
 *    sys time   39.13 secs    1.61 millis   39.12 secs
 *
 * copy_file2
 * time ./cp.o big-file test2
 *  ________________________________________________________
 * Executed in   14.19 secs    fish           external
 *   usr time    0.15 secs    2.35 millis    0.15 secs
 *   sys time   11.47 secs    5.06 millis   11.47 secs

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
  copy_files2(ring, in_fd, out_fd);

  io_uring_queue_exit(&ring);

  return 0;
}
