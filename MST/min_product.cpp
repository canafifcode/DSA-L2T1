// minimum product spanning tree using krushkal's algorithm
#include <bits/stdc++.h>
using namespace std;
double totalWeight = 1.0;
/*
Thinking process:
1. Since we need to minimize the product of weights, we can take the logarithm of the weights.
2. The logarithm of a product is the sum of the logarithms.
3. Therefore, minimizing the product of weights is equivalent to minimizing the sum of the logarithms of the weights.
4. We can then apply Kruskal's algorithm on these logarithmic weights to find the MST.
*/

int findParent(int u, vector<int> &parent)
{
    if (parent[u] == u)
        return u;
    return parent[u] = findParent(parent[u], parent);
}

void unionSets(int u, int v, vector<int> &parent, vector<int> &rank)
{
    int pu = findParent(u, parent);
    int pv = findParent(v, parent);

    if (pu != pv)
    {
        if (rank[pu] < rank[pv])
        {
            parent[pu] = pv;
        }
        else if (rank[pu] > rank[pv])
        {
            parent[pv] = pu;
        }
        else
        {
            parent[pv] = pu;
            rank[pu]++;
        }
    }
    else
        return;
}

vector<tuple<int, int, double>> krushkal(int n, vector<tuple<int, int, double>> &edges)
{
    // sorting edges on logarithmic weight
    sort(edges.begin(), edges.end(), [](const auto &a, const auto &b)
         { return get<2>(a) < get<2>(b); });

    vector<int> parent(n);
    vector<int> rank(n, 0);

    // parent array
    for (int i = 0; i < n; i++)
        parent[i] = i;

    vector<tuple<int, int, double>> mst;

    for (auto &edge : edges)
    {
        int u = get<0>(edge);
        int v = get<1>(edge);
        double logWeight = get<2>(edge);

        int pu = findParent(u, parent);
        int pv = findParent(v, parent);

        if (pu != pv)
        {
            unionSets(u, v, parent, rank);
            mst.push_back({u, v, logWeight});
            totalWeight *= exp(logWeight); // multiply the actual weights
        }
    }

    return mst;
}

int main()
{
    int n, m;
    cin >> n >> m;

    vector<tuple<int, int, double>> edges;

    for (int i = 0; i < m; i++)
    {
        int u, v;
        double w;
        cin >> u >> v >> w;
        u--;
        v--;
        edges.push_back({u, v, log(w)}); // store logarithmic weight
    }

    vector<tuple<int, int, double>> mst = krushkal(n, edges);

    cout << "Total product weight of MST: " << totalWeight << "\n";
    cout << "Edges in the MST:\n";
    for (auto &edge : mst)
    {
        cout << get<0>(edge) + 1 << " " << get<1>(edge) + 1 << " with log weight " << get<2>(edge) << "\n";
    }

    return 0;
}