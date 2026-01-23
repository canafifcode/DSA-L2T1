//finding second best mst
// kruskal's algorithm for finding minimum spanning tree
/*
Thinking Process:
1. Find the minimum spanning tree (MST) of the graph.
2. For each edge in the MST, temporarily remove it and find the new MST.
3. The second best MST is the one with the minimum total weight among all such possibilities.
*/
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

    vector<tuple<int, int, int>> edges;

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({w, u, v});
    }

    vector<tuple<int, int, int>> mst = krushkal(n, edges);

    cout << "Total weight of MST: " << totalWeight << "\n";
    cout << "Edges in the MST:\n";
    for (auto &edge : mst)
    {
        cout << get<1>(edge) << " " << get<2>(edge) << " with weight " << get<0>(edge) << "\n";
    }

    // Finding second best MST
    int secondBestWeight = INT_MAX;

    for (auto &removedEdge : mst)
    {
        int ru = get<1>(removedEdge);
        int rv = get<2>(removedEdge);
        int rweight = get<0>(removedEdge);

        // Re-initialize union-find structure
        vector<int> parent(n);// parent array
        vector<int> rank(n, 0);// rank array-> for union by rank
        for (int i = 0; i < n; i++)
            parent[i] = i;

        int currentWeight = 0;
        int edgesUsed = 0;

        for (auto &edge : edges)
        {
            int weight = get<0>(edge);
            int u = get<1>(edge);
            int v = get<2>(edge);

            // Skip the removed edge
            if ((u == ru && v == rv && weight == rweight) || (u == rv && v == ru && weight == rweight))
                continue;

            int pu = findParent(u, parent);
            int pv = findParent(v, parent);

            if (pu != pv)
            {
                unionSets(u, v, parent, rank);
                currentWeight += weight;
                edgesUsed++;
            }
        }

        // Check if we formed a valid MST
        if (edgesUsed == n - 1)
        {
            secondBestWeight = min(secondBestWeight, currentWeight);
        }
    }

    if (secondBestWeight != INT_MAX)
    {
        cout << "Total weight of Second Best MST: " << secondBestWeight << "\n";
    }
    else
    {
        cout << "No Second Best MST exists.\n";
    }
    return 0;
}