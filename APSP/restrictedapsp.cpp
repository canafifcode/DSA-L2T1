#include <bits/stdc++.h>

using namespace std;

/*
FORBIDDEN CITIES SHORTEST PATH

Some cities are closed.
Answer shortest path queries without passing through
any forbidden node.
*/

const int INF = 1e9; // A large value representing infinity

void floydWarshall(vector<vector<int>> &dist, vector<bool> &forbidden)
{
    int V = dist.size();

    for (int k = 0; k < V; ++k)
    {
        if (forbidden[k])
            continue; // skip forbidden

        for (int i = 0; i < V; ++i)
        {
            if (forbidden[i])
                continue; // skip forbidden

            for (int j = 0; j < V; ++j)
            {
                if (forbidden[j])
                    continue; // skip forbidden
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

    // Read forbidden cities
    int q;
    cin >> q;

    for (int i = 0; i < q; i++)
    {
        int k; // forbidden city number
        cin >> k;

        vector<bool> forbidden(V, false);
        for (int j = 0; j <= k; j++)
        {
            int city;
            cin >> city;
            forbidden[city] = true;
        }

        int start, end;
        cin >> start >> end;

        if (forbidden[start] || forbidden[end])
        {
            cout << "-1" << endl;
            continue;
        }

        // Run Floyd-Warshall algorithm
        vector<vector<int>> tempDist = dist; // Copy original distances
        floydWarshall(tempDist, forbidden);

        if (tempDist[start][end] >= INF)
        {
            cout << "-1" << endl;
        }
        else
        {
            cout << tempDist[start][end] << endl;
        }
    }

    return 0;
}