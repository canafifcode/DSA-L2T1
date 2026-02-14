#include <bits/stdc++.h>

using namespace std;

struct Edge {
    int to;
    int capacity;
    int flow;
    int rev;
    bool is_original; // To distinguish from backward edges during min-cut check
};

vector<vector<Edge>> adj;
vector<int> parent_node;
vector<int> edge_idx;

// BFS to find an augmenting path
bool bfs(int s, int t) {
    parent_node.assign(adj.size(), -1);
    edge_idx.assign(adj.size(), -1);

    queue<int> q;
    q.push(s);
    parent_node[s] = s;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int i = 0; i < (int)adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (parent_node[e.to] == -1 && e.capacity - e.flow > 0) {
                parent_node[e.to] = u;
                edge_idx[e.to] = i;
                if (e.to == t) return true;
                q.push(e.to);
            }
        }
    }
    return false;
}

int maxFlow(int s, int t) {
    int total_flow = 0;
    while (bfs(s, t)) {
        int path_flow = INT_MAX;

        // Find bottleneck capacity
        for (int v = t; v != s; v = parent_node[v]) {
            int u = parent_node[v];
            int idx = edge_idx[v];
            path_flow = min(path_flow, adj[u][idx].capacity - adj[u][idx].flow);
        }

        // Update residual graph
        for (int v = t; v != s; v = parent_node[v]) {
            int u = parent_node[v];
            int idx = edge_idx[v];
            int rev_idx = adj[u][idx].rev;

            adj[u][idx].flow += path_flow;
            adj[v][rev_idx].flow -= path_flow;
        }
        total_flow += path_flow;
    }
    return total_flow;
}

void addEdge(int u, int v, int cap) {
    // Forward edge: is_original = true
    adj[u].push_back({v, cap, 0, (int)adj[v].size(), true});
    // Backward edge: is_original = false
    adj[v].push_back({u, 0, 0, (int)adj[u].size() - 1, false});
}

int main() {
    int n, m;
    if (!(cin >> n >> m)) return 0;

    adj.assign(n + 1, vector<Edge>()); // Using 1-based indexing for convenience

    for (int i = 0; i < m; i++) {
        int u, v, cap;
        cin >> u >> v >> cap;
        addEdge(u, v, cap);
    }

    int s, t;
    cin >> s >> t;

    // 1. Calculate Max Flow
    int total_max_flow = maxFlow(s, t);

    // 2. Find reachable nodes from source in the residual graph (Set S)
    vector<bool> visited(n + 1, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto &e : adj[u]) {
            if (!visited[e.to] && e.capacity - e.flow > 0) {
                visited[e.to] = true;
                q.push(e.to);
            }
        }
    }

    // 3. Output results
    cout << "\n--- Results ---" << endl;
    cout << "Maximum Flow: " << total_max_flow << endl;

    int min_cut_sum = 0;
    cout << "Min-Cut Edges (Edges from Set S to Set T):" << endl;
    for (int u = 1; u <= n; u++) {
        if (visited[u]) { // Node is in Set S
            for (auto &e : adj[u]) {
                // If the edge was an original input edge and its destination is NOT in S
                if (e.is_original && !visited[e.to]) {
                    cout << u << " -> " << e.to << " with capacity " << e.capacity << endl;
                    min_cut_sum += e.capacity;
                }
            }
        }
    }

    cout << "Sum of Min-Cut capacities: " << min_cut_sum << endl;
    cout << "(Verified: " << (total_max_flow == min_cut_sum ? "Success" : "Error") << ")" << endl;

    return 0;
}