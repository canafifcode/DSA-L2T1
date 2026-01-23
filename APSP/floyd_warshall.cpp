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

    // Run Floyd-Warshall algorithm
    floydWarshall(dist);

    // Output the distance matrix
    for (int i = 0; i < V; ++i)
    {
        for (int j = 0; j < V; ++j)
        {
            if (dist[i][j] == INF)
            {
                cout << "INF ";
            }
            else
            {
                cout << dist[i][j] << " ";
            }
        }
        cout << endl;
    }

    return 0;
}