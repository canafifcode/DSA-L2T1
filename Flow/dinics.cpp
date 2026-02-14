#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, cap, flow, rev;
};

vector<vector<Edge>> adj;
vector<int> level;
vector<int> ptr; // To keep track of the next edge to explore in DFS

void addEdge(int u, int v, int cap) {
    adj[u].push_back({v, cap, 0, (int)adj[v].size()});
    adj[v].push_back({u, 0, 0, (int)adj[u].size() - 1});
}

// BFS to build the Level Graph
bool bfs(int s, int t) {
    fill(level.begin(), level.end(), -1);
    level[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (auto& edge : adj[v]) {
            if (edge.cap - edge.flow > 0 && level[edge.to] == -1) {
                level[edge.to] = level[v] + 1;
                q.push(edge.to);
            }
        }
    }
    return level[t] != -1;
}

// DFS to find Blocking Flow in the Level Graph
int dfs(int v, int t, int pushed) {
    if (pushed == 0) return 0;
    if (v == t) return pushed;

    // Start from the last explored edge (ptr[v]) to avoid O(E) per DFS call
    for (int& cid = ptr[v]; cid < adj[v].size(); ++cid) {
        auto& edge = adj[v][cid];
        int tr = edge.to;
        if (level[v] + 1 != level[tr] || edge.cap - edge.flow == 0) continue;
        
        int tr_pushed = dfs(tr, t, min(pushed, edge.cap - edge.flow));
        if (tr_pushed == 0) continue;

        edge.flow += tr_pushed;
        adj[tr][edge.rev].flow -= tr_pushed;
        return tr_pushed;
    }
    return 0;
}

int dinic(int s, int t) {
    int flow = 0;
    while (bfs(s, t)) {
        fill(ptr.begin(), ptr.end(), 0);
        while (int pushed = dfs(s, t, INT_MAX)) {
            flow += pushed;
        }
    }
    return flow;
}

int main() {
    int n, m;
    cin >> n >> m;
    adj.assign(n + 1, vector<Edge>());
    level.resize(n + 1);
    ptr.resize(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        addEdge(u, v, c);
    }

    int s, t;
    cin >> s >> t;
    cout << "Max Flow: " << dinic(s, t) << endl;

    return 0;
}