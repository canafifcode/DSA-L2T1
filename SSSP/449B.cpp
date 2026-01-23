/*
Jzzhu is the president of country A. There are n cities
numbered from 1 to n in his country. City 1 is the
capital of A. Also there are m roads connecting the
cities. One can go from city ui to vi (and vise versa)
using the i-th road, the length of this road is xi.
Finally, there are k train routes in the country. One
can use the i-th train route to go from capital of the
country to city si (and vise versa), the length of this
route is yi.

Jzzhu doesn't want to waste the money of the country,
so he is going to close some of the train routes.
Please tell Jzzhu the maximum number of the train routes
which can be closed under the following condition:
the length of the shortest path from every city to the
capital mustn't change.
*/

#include <bits/stdc++.h>

using namespace std;

// Dijkstra's algorithm to find shortest paths from a source node
// Parameters: n (number of nodes), source (starting node), adj (adjacency list)
// Returns: vector of shortest distances from source to all nodes
vector<long long> dijkstra(int n, int source, vector<vector<pair<int, long long>>> &adj)
{
    const long long INF = (1LL << 60);  // Initialize infinity as 2^60
    vector<long long> dist(n + 1, INF); // Distance array initialized to INF

    // Min-heap priority queue: stores (distance, node) pairs
    // greater<> comparator makes it a min-heap
    priority_queue<
        pair<long long, int>,
        vector<pair<long long, int>>,
        greater<pair<long long, int>>>
        pq;

    dist[source] = 0;     // Source distance is 0
    pq.push({0, source}); // Add source to priority queue

    // TODO: Continue with main Dijkstra loop
    while (!pq.empty())
    {
        auto p = pq.top();
        long long d = p.first;
        int u = p.second;

        pq.pop();

        if (d != dist[u])
            continue;

        for (auto &edge : adj[u])
        {
            int v = edge.first;
            long long weight = edge.second;

            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

int main()
{
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> u(m);
    vector<int> v(m);
    vector<int> x(m);

    for (int i = 0; i < m; i++)
    {
        cin >> u[i] >> v[i] >> x[i];
    }

    vector<int> s(k);
    vector<int> y(k);
    for (int i = 0; i < k; i++)
    {
        cin >> s[i] >> y[i];
    }

    vector<vector<pair<int, long long>>> adj(n + 1);

    for (int i = 0; i < m; i++)
    {
        adj[u[i]].push_back({v[i], x[i]});
        adj[v[i]].push_back({u[i], x[i]});
    }
    for (int i = 0; i < k; i++)
    {
        adj[1].push_back({s[i], y[i]});
        adj[s[i]].push_back({1, y[i]});
    }
    vector<long long> dist = dijkstra(n, 1, adj);

    vector<int> hasRoad(n + 1, 0);

    for (int i = 0; i < m; i++)
    {
        if (dist[u[i]] + x[i] == dist[v[i]])
            hasRoad[v[i]] = 1;
        if (dist[v[i]] + x[i] == dist[u[i]])
            hasRoad[u[i]] = 1;
    }

    vector<int> trainKept(n + 1, 0);
    int removable = 0;

    for (int i = 0; i < k; i++)
    {
        int city = s[i];
        long long w = y[i];

        if (w > dist[city])
        {
            removable++;
        }
        else if (w == dist[city])
        {
            if (hasRoad[city])
            {
                removable++;
            }
            else
            {
                if (!trainKept[city])
                    trainKept[city] = 1; // keep exactly one
                else
                    removable++;
            }
        }
    }
    cout << removable << "\n";

}
