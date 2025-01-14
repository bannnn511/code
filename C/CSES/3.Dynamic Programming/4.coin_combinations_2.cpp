//
// Created by ban on 14/1/25.
//

#include <cstdio>
#include <limits>

typedef long long ll;
constexpr int maxN = 1e6 + 1;
constexpr ll MOD = 1e9 + 7;


int main() {
    int n, x;
    scanf("%d %d", &n, &x);


    int a[maxN];
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    ll *ready = new ll[maxN];
    ready[0] = 1;
    for (int i = 0; i < n; i++) {
        for (int k = 1; k <= x; k++) {
            if (k < a[i]) {
                continue;
            }

            ready[k] = (ready[k] + ready[k - a[i]]) % MOD;
        }
    }

    printf("%lld\n", ready[x]);
    delete[] ready;

    return 0;
}
