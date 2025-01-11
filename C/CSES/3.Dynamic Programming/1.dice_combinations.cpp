//
// Created by ban on 11/1/25.
//

#include <cstdio>


const int MOD = 1e9 + 7;


int main() {
    int n;
    scanf("%d", &n);

    if (n == 0) {
        return 1;
    }

    const int maxN = 1e6;
    int dp[maxN + 1] = {};

    dp[0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= 6 && i - j >= 0; j++)
            dp[i] = (dp[i] + dp[i - j]) % MOD;

    printf("%d", dp[n]);

    return 0;
}
