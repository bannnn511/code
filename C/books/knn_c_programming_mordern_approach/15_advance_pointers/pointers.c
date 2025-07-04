#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *duplicate(const char *str) {
  char *str2 = malloc(strlen(str) + 1);
  if (str2 == NULL) {
    return NULL;
  }
  strcpy(str2, str);

  return str2;
}

int *create_array(int n, int initial_value) {
  int *new_arr = malloc(sizeof(int)*n);
  if (new_arr == NULL) {
    return NULL;
  }

  for (int i =0;i<n;i++) {
    new_arr[i] = initial_value;
  }

  return new_arr;
}

int main(int argv, char *argc[]) {
  char *world = duplicate("hello world");
  printf("%s\n", world);

  int *arr= create_array(10,2);
  for (int i = 0;i<10;i++) {
    printf("%d ", arr[i]);
  }

  free(world);
  free(arr);

  return 0;
}
