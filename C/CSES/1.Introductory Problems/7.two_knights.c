//
// Created by ban on 30/12/24.
//

#include <stdio.h>

int main() {
    long long n;
    scanf("%lld", &n);
    for (long long i = 1; i <= n; i++) {
        long long x = (i * i) * (i * i - 1) / 2 - 4 * (i - 2) * (i - 1);
        printf("%lld\n", x);
    }

    return 0;
}
