//
// Created by ban on 3/1/25.
//

#include <cstdio>
#include <vector>
#include <algorithm>

typedef long long ll;

struct event {
    ll time;
    int type;
};

// sweep line algorithm
int main() {
    ll n;
    scanf("%lld", &n);

    std::vector<event> events;
    for (ll i = 0; i < n; i++) {
        ll time1, time2;
        scanf("%lld %lld", &time1, &time2);

        events.push_back({time1, 1});
        events.push_back({time2, -1});
    }
    std::sort(
        events.begin(),
        events.end(),
        [](event a, event b) {
            return a.time == b.time ? a.type > b.type : a.time < b.time;
        });

    int result, cur;
    for (auto e: events) {
        cur += e.type;
        result = std::max(result, cur);
    }

    printf("%d", result);

    return 0;
}
