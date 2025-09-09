
#include <cstdio>
#include <set>
using namespace std;

int N, x;
set<int> s;
int main() {
  scanf("%d", &N);
  for (int i = 0; i < N; i++) {
    int a;
    scanf("%d", &a);
    s.insert(a);
  }
  printf("%zu\n", s.size());

  return 0;
}