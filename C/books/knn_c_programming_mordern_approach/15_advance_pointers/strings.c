#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

char *read_line(FILE *fp) {
  int offset = 0;
  int bufsize = 4;
  char *buf;
  int c;

  buf = malloc(bufsize);
  if (buf == NULL) {
    return NULL;
  }

  while (c = fgetc(fp), c != '\n' && c != EOF) {
    if (offset == bufsize - 1) {
      bufsize *= 2;
      char *new_buf = realloc(buf, bufsize);
      if (new_buf == NULL) {
        free(new_buf);
        return NULL;
      }

      buf = new_buf;
    }

    buf[offset++] = c;
  }

  if (c == EOF && offset == 0) {
    free(buf);
    return NULL;
  }

  if (offset < bufsize - 1) {
    char *new_buf = realloc(buf, offset + 1);
    if (new_buf == NULL) {
      free(new_buf);
      return NULL;
    }

    buf = new_buf;
  }

  buf[offset] = '\0';

  return buf;
}

char *concat(const char *s1, const char *s2) {
  char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);
  if (result == NULL) {
    printf("Error: malloc failed in concat");
    exit(EXIT_FAILURE);
  }
  strcpy(result, s1);
  strcat(result, s2);

  return result;
}

int main() {
  char *test = concat("hello", " world");
  printf("%s\n", test);

  char *s = "hello world";
  // s[0] ='z'; // undefined

  char t[] = "hello world";
  t[0] = 'z';
  printf("t: %s\n", t);

  FILE *fp = fopen("foo.txt", "r");
  if (fp == NULL) {
    perror("fopen");
    return -1;
  }

  char *line;
  while (line = read_line(fp), line != NULL) {
    printf("%s\n", line);
    free(line);
  }

  return 0;
}
