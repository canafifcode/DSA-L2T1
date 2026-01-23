#include <bits/stdc++.h>

using namespace std;

int totalWeight = 0;

int findParent(int u, vector<int> &parent)
{
    if (parent[u] == u)
        return u;
    return parent[u] = findParent(parent[u], parent);
}

void unionSets(int u, int v, vector<int> &parent, vector<int> &rank)
{
    int pu = findParent(u, parent);
    int pv = findParent(v, parent);

    if (pu != pv)
    {
        if (rank[pu] < rank[pv])
        {
            parent[pu] = pv;
        }
        else if (rank[pu] > rank[pv])
        {
            parent[pv] = pu;
        }
        else
        {
            parent[pv] = pu;
            rank[pu]++;
        }
    }
    else
        return;
}

vector<tuple<int, int, int>> krushkal(int n, vector<tuple<int, int, int>> &edges)
{
    // sorting edges on weiqht
    sort(edges.begin(), edges.end());

    vector<int> parent(n);
    vector<int> rank(n, 0);

    // parent array
    for (int i = 0; i < n; i++)
        parent[i] = i;

    vector<tuple<int, int, int>> mst;

    for (auto &edge : edges)
    {
        int weight = get<0>(edge);
        int u = get<1>(edge);
        int v = get<2>(edge);

        int pu = findParent(u, parent);
        int pv = findParent(v, parent);

        if (pu != pv)
        {
            unionSets(u, v, parent, rank);
            mst.push_back({u, v, weight});
            totalWeight += weight;
        }
    }

    return mst;
}

int main()
{
    int n, m;
    cin >> n >> m;
    vector<tuple<int, int, int>> edges; // (w, u, v)
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({w, u, v});
    }

    vector<tuple<int, int, int>> mst = krushkal(n, edges);

    cout << "Total weight " << totalWeight << endl;

    for (auto &edge : mst)
    {
        int u = get<0>(edge);
        int v = get<1>(edge);
        // int w = get<2>(edge);
        cout << u << " " << v << "\n";
    }
}