#include <stdbool.h>
#include <stdio.h>

int sum_array(const int a[], int n) {
  int i, sum;
  sum = 0;
  for (i = 0; i < n; i++) {
    sum += a[i];
  }

  return sum;
}

int sum_array2(const int a[], int n) {
  int sum;
  sum = 0;
  for (const int *p = &a[0]; p < &a[n]; p++) {
    sum += *p;
  }

  return sum;
}

bool search_key(const int a[], int n, int key) {
  for (const int *p = &a[0]; p < &a[n]; p++) {
    if (*p == key) {
      return true;
    }
  }
  return false;
}

void store_zeros(const int a[], int n) {
  int *p;
  for (p = &a[0]; p < &a[n]; p++) {
    *p = 0;
  }
}

int main(int argc, char **argv) {
  int a[] = {5, 15, 34, 54, 14, 2, 52, 72};
  int *p = &a[5];
  int *q = &a[1];

  for (int i = 0; i < 8; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");

  for (const int *k = &a[0]; k < &a[8]; k++) {
    printf("%d ", *k);
  }
  printf("\n");

  printf("%ld %d %d\n", q - p, *q, *p);

  int sum1 = sum_array(a, 8);
  int sum2 = sum_array2(a, 8);
  printf("%d == %d\n", sum1, sum2);

  int b[] = {1, 2, 4, 5, 6};
  store_zeros(b, 5);
  for (int i = 0; i < 5; i++) {
    printf("%d ", b[i]);
  }

  return 0;
}
