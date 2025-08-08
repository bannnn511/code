#ifndef CP_LIBURING_2_H
#define CP_LIBURING_2_H

#include <bits/types/struct_iovec.h>
#include <liburing.h>
#include <liburing/io_uring.h>
#include <stddef.h>

int copy_files(struct io_uring ring, int input_fd, int output_fd);
size_t get_file_size(int fd);

#endif // CP_LIBURING_2_H
