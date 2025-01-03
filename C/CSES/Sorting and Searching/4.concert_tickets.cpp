//
// Created by ban on 2/1/25.
//

#include <algorithm>
#include <cstdio>
#include <set>
#include <vector>

typedef long long ll;
const ll maxN = 2e5 + 1;


int main() {
    ll n, m;
    std::multiset<ll> S;
    scanf("%lld  %lld", &n, &m);
    for (ll i = 0; i < n; i++) {
        ll x;
        scanf("%lld", &x);
        S.insert(-x);
    }


    ll b[maxN];
    for (ll i = 0; i < m; i++) {
        scanf("%lld", &b[i]);
    }

    for (ll i = 0; i < m; i++) {
        auto it = S.lower_bound(-b[i]);
        if (it == S.end()) {
            printf("-1\n");
        } else {
            printf("%lld\n", -*it);
            S.erase(it);
        }
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
//
//
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
//     for (ll i = 0; i < m; i++) {
//         ll input;
//         scanf("%lld", &input);
//
//         // binary search
//         ll idx = search(a, n, input);
//         if (idx == -1) {
//             printf("-1\n");
//             continue;
//         }
//         printf("%lld\n", a[idx]);
//
//         // remove a[idx] from array
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
//
//
