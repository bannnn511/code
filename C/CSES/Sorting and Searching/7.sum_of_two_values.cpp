//
// Created by ban on 6/1/25.
//

#include <algorithm>
#include <cstdio>
typedef long long ll;

const ll maxN = 2e5 + 1;

typedef std::pair<ll, ll> couple;

ll search(couple *a, ll n, ll x) {
    ll left = 1, right = n;
    while (left <= right) {
        ll middle = (left + right) / 2;
        if (a[middle].first == x) {
            return a[middle].second;
        }

        if (a[middle].first > x) {
            right = middle - 1;
        } else {
            left = middle + 1;
        }
    }

    return -1;
}

int main() {
    couple a[maxN];

    ll n, x;
    scanf("%lld %lld", &n, &x);

    for (ll i = 1; i <= n; i++) {
        ll input;
        scanf("%lld", &input);
        a[i] = {input, i};
    }
    std::sort(a + 1, a + n + 1, [](couple a, couple b) {
        return a.first < b.first;
    });

    for (ll i = 1; i <= n; i++) {
        ll b = search(a, n, x - a[i].first);
        if (b != -1 && b != a[i].second) {
            printf("%lld %lld\n", a[i].second, b);
            return 0;
        }
    }

    printf("IMPOSSIBLE");

    return 0;
}
