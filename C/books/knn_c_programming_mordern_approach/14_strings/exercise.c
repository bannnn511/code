#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 100

void capitalize(char *buffer, int n) {
  for (int i = 0; i < n; i++) {
    buffer[i] = toupper(buffer[i]);
  }
}

int read_line(char *buffer, int size) {
  int ch, i = 0;

  while ((ch = getchar()) != EOF && isspace(ch) && ch != '\n');

  if (ch == EOF || ch == '\n') {
    buffer[0] = '\0';
    return 0;
  }

  if (i < size - 1)
    buffer[i++] = ch;

  while ((ch = getchar()) != EOF && ch != '\n') {
    if (i < size - 1)
      buffer[i++] = ch;
  }
  buffer[i] = '\0';

  return i;
}

int count_spaces(char *buffer, int size) {
  int count = 0;
  for (; *buffer != '\0'; buffer++)
    count++;

  return count;
}

int main(const int argc, const char *argv[]) {
  char buffer[BUFFER_SIZE];
  read_line(buffer, BUFFER_SIZE);
  printf("%s\n", buffer);

  int spaces = count_spaces(buffer, BUFFER_SIZE);
  printf("string has %d spaces\n", spaces);

  char hello[] = "hello";
  capitalize(hello, strlen(hello));
  printf("%s", hello);

  return 0;
}
