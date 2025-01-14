//
// Created by ban on 14/1/25.
//

#include <algorithm>
#include <iostream>

using namespace std;
typedef long long ll;

constexpr int maxN = 2e5;

int main() {
    int n;
    cin >> n;

    ll a[maxN] = {};
    ll sum = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        sum += a[i];
    }
    sort(a, a + n);

    ll median = a[(n - 1) / 2];

    ll steps = 0;
    for (int i = 0; i < n; i++) {
        steps += abs(a[i] - median);
    }
    cout << steps;

    return 0;
}
