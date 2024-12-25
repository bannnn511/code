//
// Created by ban on 25/12/24.
//

#include <stdio.h>
#include <string.h>

const int maxN = 1e6 + 1;

int max(int a, int b);


int main() {
    char buf[maxN];
    int cur, best = 1;

    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        return 0;
    }

    const int n = strlen(buf);
    for (int i = 0; i < n; i++) {
        if (buf[i] == buf[i - 1]) cur++;
        else cur = 1;
        best = max(best, cur);
    }


    printf("%d", best);

    return 0;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }

    return b;
}
