#include <asm/unistd_64.h>
#include <fcntl.h>
#include <linux/io_uring.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define QUEUE_DEPTH 1
#define BLOCK_SZ 1024

/* This is x86 specific */
#define read_barrier() __asm__ __volatile__("" ::: "memory")
#define write_barrier() __asm__ __volatile__("" ::: "memory")

/* Macros for barriers needed by io_uring */
#define io_uring_smp_store_release(p, v)                                       \
  atomic_store_explicit((_Atomic typeof(*(p)) *)(p), (v), memory_order_release)
#define io_uring_smp_load_acquire(p)                                           \
  atomic_load_explicit((_Atomic typeof(*(p)) *)(p), memory_order_acquire)

int ring_fd;
unsigned *sring_tail, *sring_mask, *sring_array, *cring_head, *cring_tail,
    *cring_mask;
struct io_uring_sqe *sqes;
struct io_uring_cqe *cqes;
char buff[BLOCK_SZ];
off_t offset;

struct app_io_sq_ring {};

int io_uring_setup(unsigned entries, struct io_uring_params *p) {
  return (int)syscall(__NR_io_uring_setup, entries, p);
}

int io_uring_enter(int ring_fd, unsigned int to_submit,
                   unsigned int min_complete, unsigned int flags) {
  return (int)syscall(__NR_io_uring_enter, ring_fd, to_submit, min_complete,
                      flags, NULL, 0);
}

int app_setup_uring(void) {
  struct io_uring_params p;
  void *sq_ptr, *cq_ptr;

  memset(&p, 0, sizeof(p));
  ring_fd = io_uring_setup(QUEUE_DEPTH, &p);
  if (ring_fd < 0) {
    perror("io_uring_setup");
    return 1;
  }

  int sring_sz = p.sq_off.array + p.sq_entries * sizeof(unsigned);
  int cring_sz = p.cq_off.cqes + p.cq_entries * sizeof(struct io_uring_cqe);

  if (p.features & IORING_FEAT_SINGLE_MMAP) {
    if (cring_sz > sring_sz)
      sring_sz = cring_sz;

    cring_sz = sring_sz;
  }

  sq_ptr = mmap(NULL, sring_sz, PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_POPULATE, ring_fd, IORING_OFF_SQ_RING);
  if (sq_ptr == MAP_FAILED) {
    perror("mmap1");
    return 1;
  }

  if (p.features & IORING_FEAT_SINGLE_MMAP) {
    cq_ptr = sq_ptr;
  } else {
    cq_ptr = mmap(NULL, cring_sz, PROT_READ | PROT_WRITE,
                  MAP_SHARED | MAP_POPULATE, ring_fd, IORING_OFF_CQ_RING);
    if (cq_ptr == MAP_FAILED) {
      perror("mmap2");
      return 1;
    }
  }

  sring_tail = sq_ptr + p.sq_off.tail;
  sring_mask = sq_ptr + p.sq_off.ring_mask;
  sring_array = sq_ptr + p.sq_off.array;

  sqes = mmap(NULL, p.sq_entries * sizeof(struct io_uring_sqe),
              PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE, ring_fd,
              IORING_OFF_SQES);
  if (sqes == MAP_FAILED) {
    perror("mmap3");
    return 1;
  }

  cring_head = cq_ptr + p.cq_off.head;
  cring_tail = cq_ptr + p.cq_off.tail;
  cring_mask = cq_ptr + p.cq_off.ring_mask;
  cqes = cq_ptr + p.cq_off.cqes;

  return 0;
}

int submit_to_sq(int fd, int op) {
  unsigned index, tail;

  tail = *sring_tail;
  index = tail & *sring_mask;
  struct io_uring_sqe *sqe = &sqes[index];
  sqe->opcode = op;
  sqe->fd = fd;
  sqe->addr = (unsigned long)buff;
  if (op == IORING_OP_READ) {
    memset(buff, 0, sizeof(buff));
    sqe->len = BLOCK_SZ;
  } else {
    sqe->len = strlen(buff);
  }
  sqe->off = offset;
  sring_array[index] = index;
  tail++;
  io_uring_smp_store_release(sring_tail, tail);

  int ret = io_uring_enter(ring_fd, 1, 1, IORING_ENTER_GETEVENTS);
  if (ret < 0) {
    perror("io_uring_enter");
    return -1;
  }

  return ret;
}

int read_from_cq() {
  struct io_uring_cqe *cqe;
  unsigned head;
  head = io_uring_smp_load_acquire(cring_head);

  if (head == *cring_tail)
    return -1;

  int index = head & *cring_mask;
  cqe = &cqes[index];
  if (cqe->res < 0) {
    fprintf(stderr, "Error: %s\n", strerror(abs(cqe->res)));
  }

  head++;
  io_uring_smp_store_release(cring_head, head);

  return cqe->res;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage: %s <filename> <destination>\n", argv[0]);
    exit(EXIT_FAILURE);
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

  if (app_setup_uring()) {
    fprintf(stderr, "Unable to fopen iouring");
    return -1;
  }

  int res;
  while (1) {
    submit_to_sq(fd, IORING_OP_READ);
    res = read_from_cq();
    if (res > 0) {
      submit_to_sq(fd2, IORING_OP_WRITE);
      read_from_cq();
    } else if (res == 0) {
      break;
    } else {
      fprintf(stderr, "Error: %s\n", strerror(abs(res)));
      break;
    }
    offset += res;
  }

  return 0;
}
