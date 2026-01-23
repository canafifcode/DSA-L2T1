#include <bits/stdc++.h>

using namespace std;

vector<vector<long long>> allpairshortestpath(int n, vector<vector<pair<int, int>>> &adj)
{
    // floyd warshall algorithm
    const long long inf = INT_MAX;

    vector<vector<long long>> dist(n + 1, vector<long long>(n + 1, inf));
    for (int i = 1; i <= n; i++)
        dist[i][i] = 0;

    for (int u = 1; u <= n; u++)
    {
        for (int i = 0; i < adj[u].size(); i++)
        {
            int v = adj[u][i].first;
            int w = adj[u][i].second;
            dist[u][v] = min(dist[u][v], (long long)w);
        }
    }

    for (int k = 1; k <= n; k++)
    {
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // return the entire distance matrix
    return dist;
}

int main()
{
    int n, m;
    long long q;
    cin >> n >> m >> q;

    vector<vector<pair<int, int>>> adj(n + 1);

    for (int i = 0; i < m; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
        adj[b].push_back({a, c});
    }

    vector<vector<long long>> dist = allpairshortestpath(n, adj);

    // queries
    while (q--)
    {
        int a, b;
        cin >> a >> b;
        if (dist[a][b] == INT_MAX)
            cout << -1 << "\n";
        else
            cout << dist[a][b] << "\n";
    }
}