//
// Created by ban on 31/12/24.
//

#include <algorithm>
#include <cstdio>

const long long maxN = 2e5 + 1;

int main() {
    long long n, x;
    scanf("%lld %lld", &n, &x);

    long long a[maxN];
    for (long long i = 0; i < n; i++) {
        scanf("%lld", &a[i]);
    }
    std::sort(a, a + n);

    long long count = 0, left = 0;
    for (long long right = n - 1; right >= left; right--) {
        if (a[left] + a[right] <= x) {
            left++;
        }
        count++;
    }


    printf("%lld", count);

    return 0;
}
