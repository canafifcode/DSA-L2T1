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

        for (int i = 0; i < (int)adj[u].size(); i++)
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

        for (int v = t; v != s; v = parent_node[v])
        {
            int u = parent_node[v];
            int idx = edge_idx[v];
            path_flow = min(path_flow, adj[u][idx].capacity - adj[u][idx].flow);
        }

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
    int N, K, M;
    if (!(cin >> N >> K >> M))
        return 0;

    int s = N;
    int t = N + 1;

    adj.resize(N + 2);
    parent_node.resize(N + 2);
    edge_idx.resize(N + 2);

    struct CompatibilityEdge
    {
        int u, v, list_idx;
    };
    vector<CompatibilityEdge> matches;

    for (int i = 0; i < K; i++)
    {
        addEdge(s, i, 1);
    }

    for (int i = 0; i < M; i++)
    {
        int u, v;
        cin >> u >> v;
        matches.push_back({u, v, (int)adj[u].size()});
        addEdge(u, v, 1);
    }

    for (int i = K; i < N; i++)
    {
        addEdge(i, t, 1);
    }

    int total_teams = maxFlow(s, t);
    cout << total_teams << endl;

    for (auto &me : matches)
    {

        if (adj[me.u][me.list_idx].flow == 1)
        {
            cout << me.u << " " << me.v << "\n";
        }
    }

    return 0;
}