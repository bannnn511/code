#include <stdio.h>
#include <set>

using namespace std;

int main()
{
    int n;
    multiset<int> m;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        int x;
        scanf("%d", &x);
        auto it = m.upper_bound(x);
        if (it != m.end())
        {
            m.erase(it);
        }
        m.insert(x);
    }

    printf("%ld\n", m.size());

    return 0;
}
