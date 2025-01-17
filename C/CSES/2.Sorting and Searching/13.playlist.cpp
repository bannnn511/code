//
// Created by ban on 17/1/25.
//

#include <cstdio>
#include <map>

int main() {
    int n;
    scanf("%d", &n);

    int arr[n];
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    std::map<int, int> m;
    int longest = 1;
    int left = 0;
    m[arr[0]] = 0;
    for (int i = 1; i < n; i++) {
        if (m.find(arr[i]) != m.end()) {
            left = std::max(left, m[arr[i]] + 1);
        }
        m[arr[i]] = i;
        longest = std::max(longest, i - left + 1);
    }

    printf("%d\n", longest);


    return 0;
}
