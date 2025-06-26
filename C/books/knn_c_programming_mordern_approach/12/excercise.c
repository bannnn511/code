#include <stdio.h>

void swap(int *i, int *j) {
    int temp = *i;
    *i = *j;
    *j = temp;
}


void split_time(long total_sec, int *hr, int *min, int *sec) {
    *hr = total_sec / 3600;
    total_sec -= *hr * 3600;
    *min = total_sec / 60;
    total_sec -= *min * 60;
    *sec = total_sec;
}

void find_two_largest(int a[], int n, int *largest, int *second_largest) {
    for (int i = 0; i < n; i++) {
        if (a[i] > *largest) {
            *largest = a[i];
        }
    }
}

int *find_largest(int a[], int n) {
    int *result = &a[0];
    for (int i = 0; i < n; i++) {
        if (a[i] > *result) {
            result = &a[i];
        }
    }

    return result;
}

int main() {
    int a = 10;
    int b = 20;

    swap(&a, &b);
    printf("%d %d\n", a, b);

    int hr, min, sec;
    split_time(3691, &hr, &min, &sec);
    printf("0 seconds = %d hr, %d min, %d sec\n", hr, min, sec);;

    int largest_array[] = {0, 1, 2, 3};
    int largest = 0;
    find_two_largest(largest_array, 4, &largest, NULL);
    printf("largest: %d\n", largest);

    const int *largest2 = find_largest(largest_array, 4);
    printf("largest 2: %d\n", *largest2);

    return 0;
}
