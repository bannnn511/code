//
// Created by ban on 25/12/24.
//

#include <stdio.h>

int main() {
    long n;
    scanf("%ld", &n);

    long sum = 0;
    for (int i = 0; i <= n; i++) {
        sum += i;
    }
    for (int i = 0; i < n - 1; i++) {
        long x;
        scanf("%ld", &x);
        sum -= x;
    }
    printf("%ld", sum);

    return 0;
}
