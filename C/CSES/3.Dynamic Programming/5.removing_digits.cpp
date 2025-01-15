//
// Created by ban on 15/1/25.
//

#include <algorithm>
#include <cstdio>

typedef long long ll;
constexpr int maxN = 1e6 + 1;

int main() {
    int n;
    scanf("%d", &n);

    int dp[maxN] = {};
    std::fill_n(dp + 1, n, n + 1);
    for (int i = 0; i <= n; i++) {
        int d = i;
        while (d > 0) {
            if (d % 10 != 0) {
                dp[i] = std::min(dp[i], dp[i - d % 10] + 1);
            }
            d /= 10;
        }
    }
    printf("%d", dp[n]);

    return 0;
}
