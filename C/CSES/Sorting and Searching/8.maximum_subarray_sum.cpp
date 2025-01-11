//
// Created by ban on 10/1/25.
//


#include <cstdio>

typedef long long ll;

constexpr ll maxN = 2e5 + 1;

ll max(ll a, ll b) {
    return a > b ? a : b;
}

int main() {
    ll n;
    scanf("%lld", &n);

    ll array[maxN];
    for (ll i = 0; i < n; i++) {
        scanf("%lld", &array[i]);
    }

    ll best = array[0], sum = 0;
    for (ll k = 0; k < n; k++) {
        sum += array[k];
        best = max(best, sum);
        if (sum < 0) sum = 0;
    }

    printf("%lld", best);

    return 0;
}
