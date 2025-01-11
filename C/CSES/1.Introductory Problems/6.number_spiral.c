//
// Created by ban on 26/12/24.
//

#include <stdio.h>

long long maxN = 1e6 + 1;

long long max(long long a, long long b) {
    if (a > b) {
        return a;
    }

    return b;
}

long long find_spiral(const long long row, const long long col) {
    long long y = max(row, col);
    long long x = y * y - y + 1;
    if (row == col) {
        return x;
    }

    if (row > col) {
        if (row % 2 == 0) return x + (row - col);
        return x - (row - col);
    }

    if (col % 2 == 1) return x + (col - row);

    return x - (col - row);
}

int main() {
    long long n;
    scanf("%lld", &n);

    long long result[n];
    for (long long i = 0; i < n; i++) {
        long long row, col;
        scanf("%lld %lld", &row, &col);
        result[i] = find_spiral(row, col);
    }

    for (long i = 0; i < n; i++) {
        printf("%lld\n", result[i]);
    }

    return 0;
}
