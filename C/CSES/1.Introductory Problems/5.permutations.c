//
// Created by ban on 25/12/24.
//

#include <stdio.h>
#include <stdlib.h>


int main() {
    long n;
    scanf("%ld", &n);

    if (n == 1) {
        printf("1 ");
    } else if (n <= 3) {
        printf("NO SOLUTION");
    } else {
        for (int i = 2; i <= n; i += 2) {
            printf("%d ", i);
        }
        for (int i = 1; i <= n; i += 2) {
            printf("%d ", i);
        }
    }


    return 0;
}

int exist = 0;

void print_arr(const long *arr, const int n) {
    for (int i = 0; i < n; i++) {
        printf("%ld ", arr[i]);
    }
    printf("\n");
}

int check_permutation(const long *arr, long n) {
    long prev = arr[0];


    for (int i = 1; i < n; i++) {
        if (labs(arr[i] - prev) <= 1) {
            return 0;
        }
        prev = arr[i];
    }
    exist = 1;


    return 1;
}


// permutation is implemented using Heap's algorithm
void permutation(long *arr, long n, long k) {
    if ((check_permutation(arr, n) == 1) ||
        (exist == 1) ||
        (k == 1)) {
        return;
    }

    permutation(arr, n, k - 1);

    for (int i = 0; i < k - 1; i++) {
        if (k % 2 == 0) {
            if (exist == 1) { return; }
            const long temp = arr[i];
            arr[i] = arr[k - 1];
            arr[k - 1] = temp;
        } else {
            if (exist == 1) { return; }
            const long temp = arr[0];
            arr[0] = arr[k - 1];
            arr[k - 1] = temp;
        }
        permutation(arr, n, k - 1);
    }
}
