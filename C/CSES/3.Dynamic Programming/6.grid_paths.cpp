#include <cstdio>

typedef long long ll;
const ll MOD = 1e9 + 7;
const int maxN = 1001;

int main() {
  int n;
  scanf("%d", &n);

  int squares[maxN][maxN] = {};
  int paths[maxN][maxN] = {};

  for (int i = 1; i < n + 1; i++) {
    for (int j = 1; j < n + 1; j++) {
      char c;
      scanf(" %c", &c);
      if (c == '*') {
        squares[i][j] = -1;
      } else {
        squares[i][j] = 0;
      }
    }
  }


  for (int i = 1; i < n + 1; i++) {
    for (int j = 1; j < n + 1; j++) {
      if (squares[1][1] == -1) {
        goto here;
      }
      if (i == 1 && j == 1) {
        paths[i][j] = 1;
      } else {
        if (squares[i][j] == -1) {
          paths[i][j] = 0;
        } else {
          paths[i][j] = paths[i - 1][j] + paths[i][j - 1];
          paths[i][j] %= MOD;
        }
      }
    }
  }

here:
  printf("%d", paths[n][n]);


  return 0;
}
