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


// int main() {
//     int n;
//     cin >> n;
//
//     int a[maxN] = {};
//     for (int i = 0; i < n; i++) {
//         cin >> a[i];
//     }
//
//     ll minValue = numeric_limits<ll>::max() - 1;
//     ll ready[maxN] = {};
//     fill_n(ready, maxN, -1);
//
//     for (int i = 0; i < n; i++) {
//         ll sum = 0;
//         int j;
//         for (j = 0; j < n; j++) {
//             if (ready[j] != -1) {
//                 sum = ready[j];
//                 break;
//             }
//             sum += abs(a[i] - a[j]);
//         }
//         ready[j] = sum;
//         minValue = min(sum, minValue);
//     }
//     cout << minValue;
//
//     return 0;
