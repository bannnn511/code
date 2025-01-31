#include <queue>
#include <cstdio>

using namespace std;
typedef pair<int, int> loc;


const int h[] = {1, -1, 0, 0}, v[] = {0, 0, 1, -1};

bool visited[1000][1000];
char c, dir[1000][1000], paths[1000000];
int dist[1000][1000];


int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    loc start, end;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf(" %c", &c);
            if (c == '#') visited[i][j] = true;
            else if (c == 'A') {
                start.first = i;
                start.second = j;
            } else if (c == 'B') {
                end.first = i;
                end.second = j;
            }
        }
    }

    queue<loc> q;
    visited[start.first][start.second] = true;
    q.push(start);
    while (!q.empty()) {
        loc cur = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int dx = cur.first + h[i];
            int dy = cur.second + v[i];
            if (dx < 0 || dx >= n || dy < 0 || dy >= m || visited[dx][dy]) {
                continue;
            }
            if (i == 0) dir[dx][dy] = 'D';
            else if (i == 1) dir[dx][dy] = 'U';
            else if (i == 2) dir[dx][dy] = 'R';
            else if (i == 3) dir[dx][dy] = 'L';
            dist[dx][dy] = dist[cur.first][cur.second] + 1;
            q.push({dx, dy});
            visited[dx][dy] = true;
        }
    }

    if (!visited[end.first][end.second]) {
        printf("NO\n");
        return 0;
    }

    loc cur = end;
    printf("YES\n%d\n", dist[end.first][end.second]);
    for (int i = dist[end.first][end.second]; i > 0; i--) {
        paths[i] = dir[cur.first][cur.second];
        if (paths[i] == 'D') cur = {cur.first - 1, cur.second};
        else if (paths[i] == 'L') cur = {cur.first, cur.second + 1};
        else if (paths[i] == 'U') cur = {cur.first + 1, cur.second};
        else if (paths[i] == 'R') cur = {cur.first, cur.second - 1};
    }

    for (int i = 1; i <= dist[end.first][end.second]; i++)
        printf("%c", paths[i]);
    printf("\n");
}

/*
5 8
# # # # # # # #
# . A # . . . #
# . # # . # B #
# . . . . . . #
# # # # # # # #
 */
