//
// Created by ban on 13/1/25.
//

#include <cstdio>
#include <algorithm>
#include <limits>

constexpr int maxN = 1e6;

int main() {
    int n, x;
    scanf("%d %d", &n, &x);

    int a[maxN];
    for (int i = 0; i < n; i++) {
        scanf("%intd", &a[i]);
    }

    constexpr int INF = std::numeric_limits<int>::max() - 1;

    int ready[maxN] = {};
    for (int k = 1; k <= x; k++) {
        ready[k] = INF;
        for (int i = 0; i < n; i++) {
            if (k >= a[i]) {
                ready[k] = std::min(ready[k], ready[k - a[i]] + 1);
            }
        }
    }
    printf("%d", ready[x] == INF ? -1 : ready[x]);

    return 0;
}
