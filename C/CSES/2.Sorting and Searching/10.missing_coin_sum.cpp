//
// Created by ban on 15/1/25.
//

#include <algorithm>
#include <cstdio>

typedef long long ll;
constexpr int maxN = 2e5;

int main() {
    int n;

    scanf("%d", &n);

    ll coins[maxN] = {};
    for (int i = 0; i < n; i++) {
        scanf("%lld", &coins[i]);
    }

    std::sort(coins, coins + n);
    ll sum = 1;
    for (int i = 0; i < n; i++) {
        if (coins[i] > sum) {
            break;
        }
        sum += coins[i];
    }
    printf("%lld", sum);

    return 0;
}
