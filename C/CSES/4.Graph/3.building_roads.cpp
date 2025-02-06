//
// Created by ban on 6/2/25.
//

#include <cstdio>
#include <vector>

using namespace std;

class UnionFind {
private:
  vector<int> id;
  vector<int> rank;
  int count;
  int size;

public:
  explicit UnionFind(int n) : id(n + 1), rank(n + 1, 0), count(n), size(n) {
    for (int i = 1; i <= n; i++) {
      id[i] = i;
    }
  }

  int find(int p) {
    while (p != id[p]) {
      id[p] = id[id[p]]; // Path compression
      p = id[p];
    }
    return p;
  }

  void unite(int p, int q) {
    int rootP = find(p);
    int rootQ = find(q);

    if (rootP == rootQ)
      return;

    // Union by rank
    if (rank[rootP] < rank[rootQ]) {
      id[rootP] = rootQ;
    } else if (rank[rootP] > rank[rootQ]) {
      id[rootQ] = rootP;
    } else {
      id[rootQ] = rootP;
      rank[rootP]++;
    }
    count--;
  }

  bool connected(int p, int q) { return find(p) == find(q); }

  vector<int> getIds() const {
    vector<int> result;
    for (int i = 1; i <= size; i++) {
      if (id[i] == i) {
        result.push_back(i);
      }
    }
    return result;
  }

  int getCount() {
    return this->count;
  }
};

int main() {
  int n;
  int m;

  scanf("%d %d", &n, &m);
  UnionFind uf = UnionFind(n);
  for (int i = 1; i <= m; i++) {
    int x, y;
    scanf("%d %d", &x, &y);
    if (uf.connected(x, y)) {
      continue;
    }
    uf.unite(x, y);
  }

  vector<int> ids = uf.getIds();
  printf("%d\n", uf.getCount() - 1);
  for (int i = 1; i < ids.size(); i++) {
    printf("%d %d\n", ids[i - 1], ids[i]);
  }

  return 0;
}
