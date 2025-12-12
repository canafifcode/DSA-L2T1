#include <bits/stdc++.h>

using namespace std;

long long dijkstra(int n, int source, vector<vector<pair<int, int>>> &adj)
{
    const long long infinity = (1LL << 60);
    vector<vector<long long>> dist(n + 1, vector<long long>(2, infinity));
    priority_queue<tuple<long long, int, int>,
                   vector<tuple<long long, int, int>>,
                   greater<tuple<long long, int, int>>>
        pq;

    dist[source][0] = 0;
    pq.push({0, source, 0}); // cost port coupon

    while (!pq.empty())
    {
        auto t = pq.top();
        long long c = get<0>(t);
        int p = get<1>(t);
        int u = get<2>(t);
        pq.pop();
        if (c > dist[p][u])
            continue;
        for (auto &e : adj[p])
        {
            int next = e.first;
            long long next_cost = e.second;

            // without coupon
            long long new_cost_nocoupon = c + next_cost;
            if (new_cost_nocoupon < dist[next][u])
            {
                dist[next][u] = new_cost_nocoupon;
                pq.push({new_cost_nocoupon, next, u});
            }

            // with coupon
            if (u == 0)
            {
                long long new_cost_yescoupon = c + next_cost / 2;
                if (new_cost_yescoupon < dist[next][1])
                {
                    dist[next][1] = new_cost_yescoupon;
                    pq.push({new_cost_yescoupon, next, 1});
                }
            }
        }
    }
    return min(dist[n][0], dist[n][1]);
}

int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n + 1);

    for (int i = 0; i < m; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
        // adj[b].push_back({a, c});
    }

    long long bestCost = dijkstra(n, 1, adj);
    cout << bestCost << "\n";

    return 0;
}