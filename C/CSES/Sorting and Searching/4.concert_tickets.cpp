//
// Created by ban on 2/1/25.
//

#include <algorithm>
#include <cstdio>
#include <vector>

typedef long long ll;
const ll maxN = 2e5 + 1;

int main() {
    ll n, m;
    std::vector<ll> V;
    scanf("%lld  %lld", &n, &m);
    for (ll i = 0; i < n; i++) {
        ll x;
        scanf("%lld", &x);
        V.push_back(x);
    }

    std::sort(V.begin(), V.end());

    ll b[maxN];
    for (ll i = 0; i < m; i++) {
        scanf("%lld", &b[i]);
    }

    for (ll i = 0; i < m; i++) {
        auto it = std::lower_bound(V.begin(), V.end(), b[i]);
        if ((it == V.begin() && *it > b[i]) || V.empty()) {
            printf("-1\n");
            continue;
        }
        if (it != V.end() && *it == b[i]) {
            printf("%lld\n", *it);
        } else {
            --it;
            printf("%lld\n", *it);
        }
        V.erase(it);
    }


    return 0;
}


// ll search(ll *a, ll n, ll x) {
//     ll k = n - 1;
//     for (ll b = n / 2; b > 0; b /= 2) {
//         while (k - b >= 0 && a[k - b] >= x) {
//             k -= b;
//         }
//     }
//     if (a[k] > x) {
//         return k - 1;
//     }
//
//     return k;
// }


// int main() {
//     ll n, m;
//     ll a[maxN];
//     scanf("%lld  %lld", &n, &m);
//     for (ll i = 0; i < n; i++) {
//         scanf("%lld", &a[i]);
//     }
//
//     std::sort(a, a + n);
//
//     ll b[maxN];
//     for (ll i = 0; i < m; i++) {
//         scanf("%lld", &b[i]);
//     }
//
//     for (ll i = 0; i < m; i++) {
//         ll idx = search(a, n, b[i]);
//         if (idx == -1) {
//             printf("-1\n");
//             continue;
//         }
//         printf("%lld\n", a[idx]);
//         if (idx < n) {
//             for (ll j = idx; j < n; j++) {
//                 a[j] = a[j + 1];
//             }
//             n--;
//         }
//     }
//
//
//     return 0;
// }
