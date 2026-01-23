#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

struct Edge {
    int u, v;
    long long w;
};

// ---------- BELL-MAN FORD FUNCTION ----------
bool bellman_ford(
    int n, int source,
    const vector<Edge> &edges,
    vector<long long> &dist,
    vector<int> &parent
) {
    dist.assign(n + 1, INF);
    parent.assign(n + 1, -1);

    dist[source] = 0;

    // relax all edges (n-1 times)
    for (int i = 1; i <= n - 1; i++) {
        bool changed = false;
        for (auto &e : edges) {
            if (dist[e.u] == INF) continue;
            
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = e.u;
                changed = true;
            }
        }
        if (!changed) break; // optimization: early stop
    }

    // check for negative cycle
    for (auto &e : edges) {
        if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v])
            return false; // negative cycle detected
    }

    return true; // no negative cycle
}

// ---------- MAIN FUNCTION ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++)
        cin >> edges[i].u >> edges[i].v >> edges[i].w;

    int source = 1, target = n;
    vector<long long> dist;
    vector<int> parent;

    bool ok = bellman_ford(n, source, edges, dist, parent);

    if (!ok) {
        cout << "Negative cycle detected\n";
        return 0;
    }

    if (dist[target] == INF) {
        cout << "No path\n";
        return 0;
    }

    // -------- PATH RECONSTRUCTION --------
    vector<int> path;
    for (int v = target; v != -1; v = parent[v])
        path.push_back(v);
    reverse(path.begin(), path.end());

    // -------- OUTPUT --------
    cout << "Shortest distance: " << dist[target] << "\n";
    cout << "Path: ";
    for (int v : path) cout << v << " ";
    cout << "\n";

    return 0;
}
