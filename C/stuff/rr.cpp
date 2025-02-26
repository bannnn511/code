#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
using namespace std;

struct request {
    int idx;
    int arrive;
    int complete_time;
};


struct server {
    int idx;
    int completion_time;

    bool operator<(const server &other) const {
        if (completion_time == other.completion_time) {
            return idx < other.idx;
        }
        return completion_time > other.completion_time;
    }
};


int main() {
    // int n = 3;
    // const int m = 5;
    // const int arrival[] = {2, 4, 1, 8, 9};
    // const int burst_time[] = {7, 9, 2, 4, 5};

    int n = 4;
    const int m = 5;
    const int arrival[] = {3, 5, 1, 6, 8};
    const int burst_time[] = {9, 2, 10, 4, 5};

    set<server> servers;
    vector<request> reqs(m);

    for (int i = 0; i < m; i++) {
        reqs[i].idx = i;
        reqs[i].arrive = arrival[i];
        reqs[i].complete_time = arrival[i] + burst_time[i];
    }

    for (int i = 0; i < n; i++) {
        servers.insert({i + 1, 0});
    }
    // for (auto s: servers) {
    //     printf("%d %d\n", s.idx, s.completion_time);
    // }
    //
    sort(reqs.begin(), reqs.end(), [](request a, request b) {
        if (a.arrive == b.arrive) {
            return a.idx < b.idx;
        }
        return a.arrive < b.arrive;
    });


    int results[m] = {};
    for (int i = 0; i < m; i++) {
        server lookup_server = {0, reqs[i].arrive};
        if (auto it = servers.lower_bound(lookup_server); it == servers.end()) {
            results[reqs[i].idx] = -1;
        } else {
            results[reqs[i].idx] = it->idx;
            servers.insert({it->idx, reqs[i].complete_time});
            servers.erase(it);
        }
    }

    for (const int result: results) {
        printf("%d ", result);
    }


    return 0;
}
