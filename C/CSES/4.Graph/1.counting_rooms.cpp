//
// Created by ban on 29/1/25.
//


#include <cstdio>
using namespace std;

int visited[1000][1000];
int n, m;

int v[] = {-1, 1, 0, 0};
int h[] = {0, 0, -1, 1};

void dfs(int i, int j) {
    if (i < 0 || j < 0 || i >= n || j >= m || visited[i][j]) return;

    visited[i][j] = 1;
    for (int k = 0; k < 4; k++) {
        int dx = i + v[k];
        int dy = j + h[k];
        dfs(dx, dy);
    }
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char c;
            scanf(" %c", &c);
            visited[i][j] = c == '#';
        }
    }

    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (!visited[i][j]) {
                dfs(i, j);
                count++;
            }
        }
    }

    printf("%d\n", count);

    return 0;
}


/*
# # # # # # # #
# . . # . . . #
# # # # . # . #
# . . # . . . #
# # # # # # # #
 */
