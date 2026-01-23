#include <bits/stdc++.h>

using namespace std;

const int INF = 1e9; // A large value representing infinity

void floydWarshall(vector<vector<int>> &dist)
{
    int V = dist.size();
    for (int k = 0; k < V; ++k)
    {
        for (int i = 0; i < V; ++i)
        {
            for (int j = 0; j < V; ++j)
            {
                if (dist[i][k] < INF && dist[k][j] < INF)
                {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

int main()
{
    int V, E;
    cin >> V >> E;

    // Initialize distance matrix
    vector<vector<int>> dist(V, vector<int>(V, INF));
    for (int i = 0; i < V; ++i)
    {
        dist[i][i] = 0; // Distance to self is zero
    }

    // Read edges
    for (int i = 0; i < E; ++i)
    {
        int u, v, w;
        cin >> u >> v >> w;
        dist[u][v] = w; // Directed graph
    }

    // must visit nodes
    int must1, must2;
    cin >> must1 >> must2;

    // queries
    int Q;
    cin >> Q;

    // Run Floyd-Warshall algorithm
    floydWarshall(dist);

    // Output the distance matrix
    for (int i = 0; i < Q; ++i)
    {
        int start, end;
        cin >> start >> end;

        //place to edit
        long long ans = INF;

        if (dist[start][must1] < INF && dist[must1][end] < INF)
            ans = dist[start][must1] + dist[must1][end];

        if (dist[start][must2] < INF && dist[must2][end] < INF)
            ans = min(ans, (long long)dist[start][must2] + dist[must2][end]);

        if (ans >= INF)
            cout << -1 << "\n";
        else
            cout << ans << "\n";
    }

    return 0;
}
