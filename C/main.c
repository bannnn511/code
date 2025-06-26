#include <stdio.h>

int main(void) {
    printf("Hello, World!\n");
    int *a = NULL;
    printf("%p", a);

    int i;
    int *p, *q = &i;

    return 0;
}

void avg__sum(double a[], int n, double *avg, double *sum) {
    int i;
    *sum = 0.0;
    for (i = 0; i < n; i++)
        *sum += a[i];
    *avg = *sum / n;
}
