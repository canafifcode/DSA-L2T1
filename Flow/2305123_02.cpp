#include <bits/stdc++.h>

using namespace std;

struct Edge
{
    int to;
    int capacity;
    int flow;
    int rev;
};

vector<vector<Edge>> adj;
vector<int> parent_node;
vector<int> edge_idx;

bool bfs(int s, int t)
{
    parent_node.assign(parent_node.size(), -1);
    edge_idx.assign(edge_idx.size(), -1);

    queue<int> q;

    q.push(s);
    parent_node[s] = s;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int i = 0; i < adj[u].size(); i++)
        {
            Edge &e = adj[u][i];
            if (parent_node[e.to] == -1 && e.capacity - e.flow > 0)
            {
                parent_node[e.to] = u;
                edge_idx[e.to] = i;
                if (e.to == t)
                    return true;
                q.push(e.to);
            }
        }
    }
    return false;
}

int maxFlow(int s, int t)
{
    int total_flow = 0;
    while (bfs(s, t))
    {
        int path_flow = INT_MAX;

        // bottleneck
        for (int v = t; v != s; v = parent_node[v])
        {
            int u = parent_node[v];
            int idx = edge_idx[v];
            path_flow = min(path_flow, adj[u][idx].capacity - adj[u][idx].flow);
        }

        // Updt flows
        for (int v = t; v != s; v = parent_node[v])
        {
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

void addEdge(int u, int v, int cap)
{
    Edge forward = {v, cap, 0, (int)adj[v].size()};
    Edge backward = {u, 0, 0, (int)adj[u].size()};
    adj[u].push_back(forward);
    adj[v].push_back(backward);
}

int main() {
    int N, M;
    if (!(cin >> N >> M)) return 0;

    int s = 1; // Uttara
    int t = N; // Motijheel

    adj.assign(N + 1, vector<Edge>());
    parent_node.resize(N + 1);
    edge_idx.resize(N + 1);

    for (int i = 0; i < M; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        addEdge(u, v, c);
    }

    // Calculate initial max flow
    int base_flow = maxFlow(s, t);

    int P;
    cin >> P;
    bool found = false;

    for (int i = 1; i <= P; i++) {
        int u, v, c;
        cin >> u >> v >> c;

        // 1. Save the current state of flows
        vector<vector<Edge>> backup_adj = adj;

        // 2. Add the proposed flyover
        addEdge(u, v, c);

        // 3. If BFS finds even ONE path, the max flow will increase
        if (bfs(s, t)) {
            if (!found) found = true;
            cout << i << " ";
        }

        // 4. Restore the graph for the next independent proposal
        adj = backup_adj;
    }

    if (!found) cout << "None";
    cout << endl;

    return 0;
}