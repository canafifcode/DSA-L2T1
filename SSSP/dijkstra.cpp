#include <bits/stdc++.h>

using namespace std;

long long normal_dijkstra(int number_of_nodes, int s, vector<vector<pair<int, int>>> &adj, vector<long long> &dist, vector<int> &parent)
{
    const long long inf = INT64_MAX;
    // vector<long long> dist(number_of_nodes + 1, inf); //+1 for 1-based indexing
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    dist.assign(number_of_nodes + 1, inf);
    parent.assign(number_of_nodes + 1, -1);
    dist[s] = 0;
    pq.push({0, s}); // cost , node

    while (!pq.empty())
    {
        pair<long long, int> top = pq.top();
        long long cost = top.first;
        int node = top.second;
        pq.pop();

        if (cost > dist[node])
            continue;

        for (auto edge : adj[node])
        {
            int v = edge.first;
            int w = edge.second;
            long long new_cost = cost + w;
            if (new_cost < dist[v])
            {
                dist[v] = new_cost;
                parent[v] = node;
                pq.push({new_cost, v});
            }
        }
    }
    return dist[number_of_nodes];
}


 

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj(n + 1);

    for (int i = 0; i < m; i++)
    {
        int a, b, w;
        cin >> a >> b >> w;
        adj[a].push_back({b, w});
        // adj[b].push_back({a, w}); // for undirected
    }

    vector<long long> dist;
    vector<int> parent;

    long long shortest = normal_dijkstra(n, 1, adj, dist, parent);

    if (shortest == LLONG_MAX)
    {
        cout << "No path\n";
        return 0;
    }

    // -------- PATH CONSTRUCTION IN MAIN --------
    vector<int> path;
    for (int v = n; v != -1; v = parent[v])
        path.push_back(v);

    reverse(path.begin(), path.end());
    // -------------------------------------------

    cout << "Shortest distance: " << shortest << "\n";
    cout << "Path: ";
    for (int v : path)
        cout << v << " ";
    cout << "\n";

    return 0;
}
