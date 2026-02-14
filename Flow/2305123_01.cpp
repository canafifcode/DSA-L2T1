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

int main()
{
    int n, m;
    cin >> n >> m;

    adj.resize(n);
    parent_node.resize(n);
    edge_idx.resize(n);

    struct InputEdge
    {
        int u, v, cap, list_idx;
    };
    vector<InputEdge> input_edges;

    for (int i = 0; i < m; i++)
    {
        int u, v, cap;
        cin >> u >> v >> cap;
        input_edges.push_back({u, v, cap, (int)adj[u].size()});
        addEdge(u, v, cap);
    }

    int s, t;
    cin >> s >> t;

    int total_max_flow = maxFlow(s, t);

    cout << total_max_flow << endl;

    for (auto &ie : input_edges)
    {
        cout << ie.u << " " << ie.v << " " << adj[ie.u][ie.list_idx].flow << "/" << ie.cap << "\n";
    }

    return 0;
}