//
// Created by ban on 30/12/24.
//

#include <stdio.h>
typedef long long ll;

int main() {
    ll n;
    scanf("%lld", &n);

    int count = 0;
    int step = 5;
    for (;;) {
        int value = n / step;
        if (value == 0) {
            break;
        }
        count += value;
        step *= 5;
    }

    printf("%lld\n", count);
    return 0;
}
