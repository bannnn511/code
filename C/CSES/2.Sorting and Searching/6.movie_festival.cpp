//
// Created by ban on 3/1/25.
//

#include <cstdio>
#include <vector>
#include <algorithm>

typedef long long ll;

struct event {
    ll start;
    ll end;
};

// sweep line algorithm
int main() {
    ll n;
    scanf("%lld", &n);

    std::vector<event> events;
    for (ll i = 0; i < n; i++) {
        ll start, end;
        scanf("%lld %lld", &start, &end);
        events.push_back({start, end});
    }
    std::sort(
        events.begin(),
        events.end(),
        [](event a, event b) {
            return a.end == b.end ? a.start < b.start : a.end < b.end;
        });

    int cur = 0;
    ll prev = 0;
    for (auto e: events) {
        // printf("%lld %lld\n", e.start, e.end);
        if (prev == 0) {
            cur++;
            prev = e.end;
            continue;
        }
        if (prev > e.start) continue;
        prev = e.end;
        cur++;
    }

    printf("%d", cur);

    return 0;
}
