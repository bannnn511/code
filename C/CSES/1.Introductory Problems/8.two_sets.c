//
// Created by ban on 30/12/24.
//

#include <stdio.h>
#include <stdlib.h>

int main() {
    long long n;
    scanf("%lld", &n);


    if (n % 4 == 1 || n % 4 == 2) {
        printf("NO\n");
    } else if (n % 4 == 3) {
        printf("YES\n");
        printf("%lld\n", n / 2);
        for (long long i = 2; i <= n / 2; i += 2) {
            printf("%lld %lld ", i, n - i);
        }

        printf(" %lld\n%lld\n", n, n / 2 + 1);
        for (long long i = 1; i <= n / 2; i += 2) {
            printf("%lld %lld ", i, n - i);
        }
    } else {
        printf("YES\n");
        printf("%lld\n", n / 2);
        for (long long i = 1; i <= n / 2; i += 2) {
            printf("%lld %lld ", i, n + 1 - i);
        }

        printf("\n%lld\n", n / 2);
        for (long long i = 2; i <= n / 2; i += 2) {
            printf("%lld %lld ", i, n + 1 - i);
        }
    }


    return 0;
}

