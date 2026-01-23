#include <bits/stdc++.h>

using namespace std;
int totalWeight = 0;

vector<tuple<int, int, int>> prims(vector<vector<pair<int, int>>> adj, int root)
{
    priority_queue<tuple<int, int, int>,
                   vector<tuple<int, int, int>>,
                   greater<tuple<int, int, int>>>
        minHeap;

    int n = adj.size();
    vector<bool> visited(n, false);
    vector<tuple<int, int, int>> mst;

    minHeap.push({0, root, -1}); // rootnode
    while (!minHeap.empty())
    {
        auto top = minHeap.top();
        minHeap.pop();

        int weight = get<0>(top);
        int u = get<1>(top);
        int parent = get<2>(top);

        if (visited[u])
            continue; // visited -> continue :)

        visited[u] = true; // marking
        totalWeight += weight;
        if (weight != 0)
        {
            mst.push_back({parent, u, weight});
        }

        // adj nodes
        for (auto &edge : adj[u])
        {
            int v = edge.first;
            int w = edge.second;

            if (!visited[v])
            {
                minHeap.push({w, v, u});
            }
        }
    }
    return mst;
}

int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n + 1);

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    int root;
    cin >> root;

    vector<tuple<int, int, int>> mst = prims(adj, root);

    cout << "Total weight " << totalWeight << "\n";
    cout << "Root Node " << root << "\n";

    for (auto &edge : mst)
    {
        cout << get<0>(edge) << " " << get<1>(edge) << "\n";
    }

    return 0;
}