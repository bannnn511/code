//
// Created by ban on 31/12/24.
//

#include <cstdio>
#include <algorithm>

using namespace std;

const int maxN = 2e5 + 1;
typedef long long ll;


int main() {
    ll n, m, k;
    ll a[maxN], b[maxN];
    scanf("%lld %lld %lld", &n, &m, &k);

    for (ll i = 0; i < n; i++) {
        scanf("%lld", &a[i]);
    }

    for (ll i = 0; i < m; i++) {
        scanf("%lld", &b[i]);
    }

    sort(a, a + n);
    sort(b, b + m);

    // 6 7 8 9
    // 3 7 8
    ll count = 0;
    ll aIdx = 0, bIdx = 0;
    while (aIdx < n) {
        while (bIdx < m && b[bIdx] < a[aIdx] - k) bIdx++;
        if (bIdx < m && a[aIdx] - k <= b[bIdx] && b[bIdx] <= a[aIdx] + k) {
            count++;
            aIdx++;
            bIdx++;
        } else {
            aIdx++;
        }
    }

    printf("%lld", count);


    return 0;
}
