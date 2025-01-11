//
// Created by ban on 25/12/24.
//

#include <stdio.h>

int main() {
    long long n;
    scanf("%lld", &n);

    long prev = 0;
    long move = 0;
    for (int i = 0; i < n; i++) {
        long long x;
        scanf("%lld", &x);
        if (x - prev < 0) {
            move += prev - x;
        } else {
            prev = x;
        }
    }
    printf("%ld", move);

    return 0;
}
