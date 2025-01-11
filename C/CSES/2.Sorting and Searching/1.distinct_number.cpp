#include <set>
#include <cstdio>

using namespace std;


typedef long long ll;


int main() {
  ll n;
  scanf("%lld", &n);
  std::set<int> S;

  for (ll i = 0; i < n; i++) {
    ll x;
    scanf("%lld", &x);
    S.insert(x);
  }

  printf("%lld", (ll) S.size());

  return 0;
}
