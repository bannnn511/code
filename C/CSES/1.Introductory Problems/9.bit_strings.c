#include <stdio.h>

typedef long long ll;

#define MOD 1000000007

ll ipow(ll x, ll n) {
    if (n == 0) {
        return 1;
    }
    int result = x;
    for (ll i = 1; i < n; i++) {
        result *= x;
        result %= MOD;
    }

    return result;
}

int main() {
    ll n;
    scanf("%lld", &n);
    printf("%lld", ipow(2, n));

    return 0;
}
