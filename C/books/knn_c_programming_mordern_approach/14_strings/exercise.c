#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 100

void capitalize(char buffer[]) {
  int i = 0;
  while (buffer[i] != '\0') {
    if (isalpha(buffer[i]))
      buffer[i] = toupper(buffer[i]);

    i++;
  }
}

void capitalize1(char *buffer) {
  char *p;
  for (p = &buffer[0]; *p != '\0'; p++) {
    *p = toupper(*p);
  }
}

void censor(char str[]) {
  int i = 0;
  while (str[i + 2] != '\0') {
    if (strncmp(&str[i], "foo", 3) == 0) {
      strncpy(&str[i], "xxx", 3);
    }
    i++;
  }
}

int read_line(char *buffer, int size) {
  int ch, i = 0;

  while ((ch = getchar()) != EOF && isspace(ch) && ch != '\n')
    ;

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
  capitalize(hello);
  printf("%s\n", hello);

  char world[] = "world";
  capitalize1(world);
  printf("%s\n", world);

  char foo1[] = "food fool";
  censor(foo1);
  printf("censor foo1 %s\n", foo1);

  char *str1;
  char *str2;
  strcpy(str1, "computer");
  strcpy(str2, "science");

  if (strcmp(str1, str2) < 0) {
    strcat(str1, str2);
  } else {
    strcat(str2, str1);
  }
  str1[strlen(str1) - 6] = '\0';
  printf("%s\n", str1);

  return 0;
}
