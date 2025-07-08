#include <stdio.h>

void app_setup_uring() {}

int read_and_print(char *file_name) { return 0; }

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
