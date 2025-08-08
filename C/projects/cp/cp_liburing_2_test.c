#include "cp_liburing_2.h"
#include <fcntl.h>
#include <liburing.h>
#include <stdio.h>
#include <unistd.h>

#define QUEUE_DEPTH 1

int main() {
  const char *source_file = "test_source.txt";
  const char *destination_file = "test_destination.txt";

  // Create a test source file
  FILE *source = fopen(source_file, "w");
  if (!source) {
    perror("fopen source");
    return 1;
  }
  fprintf(source, "This is a test file for copy_files function.\n");
  fclose(source);

  // Open the source and destination files
  int input_fd = open(source_file, O_RDONLY);
  if (input_fd < 0) {
    perror("open source file");
    return 1;
  }

  int output_fd = open(destination_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (output_fd < 0) {
    perror("open destination file");
    close(input_fd);
    return 1;
  }

  // Initialize io_uring
  struct io_uring ring;
  int rc = io_uring_queue_init(QUEUE_DEPTH, &ring, 0);
  if (rc < 0) {
    perror("io_uring_queue_init");
    close(input_fd);
    close(output_fd);
    return 1;
  }

  // Call the copy_files function
  copy_files(ring, input_fd, output_fd);

  // Clean up
  io_uring_queue_exit(&ring);
  close(input_fd);
  close(output_fd);

  // Verify the output
  FILE *dest = fopen(destination_file, "r");
  if (!dest) {
    perror("fopen destination");
    return 1;
  }

  char buffer[1024];
  printf("Contents of destination file:\n");
  while (fgets(buffer, sizeof(buffer), dest)) {
    printf("%s", buffer);
  }
  fclose(dest);

  return 0;
}
