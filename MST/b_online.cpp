#include <bits/stdc++.h>
using namespace std;

class DisjointSet
{
    vector<int> rank;
    vector<int> parent, size;

public:
    DisjointSet(int n)
    {
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        for (int i = 0; i <= n; i++)
        {
            parent[i] = i;
        }
    }

    int findParent(int u)
    {
        if (u == parent[u])
            return u;
        return parent[u] = findParent(parent[u]);
    }

    void unionByRank(int u, int v)
    {
    }

    void unionBySize(int u, int v)
    {
        int parentU = findParent(u);
        int parentV = findParent(v);

        if (parentU == parentV)
            return;

        if (size[parentU] < size[parentV])
        {
            parent[parentU] = parentV;
            size[parentV] = size[parentV] + size[parentU];
        }

        else
        {
            parent[parentV] = parentU;
            size[parentU] = size[parentU] + size[parentV];
        }
    }
};

int main()
{
    int N, M;
    cin >> N >> M;

    vector<tuple<double, int, int>> edges;

    for (int i = 0; i < M; i++)
    {
        int u, v;
        double w;
        cin >> u >> v >> w;
        u--;
        v--;
        edges.push_back({w, u, v});
    }

    sort(edges.begin(), edges.end());

    double ans = INT_MAX;

    for (int i = 0; i < M; i++)
    {
        DisjointSet ds(N);
        int count = 0;
        double minimum = get<0>(edges[i]);

        for (int j = i; j < M; j++)
        {
            double w = get<0>(edges[j]);
            int u = get<1>(edges[j]);
            int v = get<2>(edges[j]);

            if (ds.findParent(u) != ds.findParent(v))
            {
                count++;
                ds.unionBySize(u, v);
                if (count == N - 1)
                {
                    ans = min(ans, w - minimum);
                    break;
                }
            }
        }
    }
    if (ans == INT_MAX)
        cout << -1 << endl;
    cout << ans << endl;
}