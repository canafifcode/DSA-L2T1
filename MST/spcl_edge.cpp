/*
Problem: You are given a graph with some "Black" edges and some "White" edges. 
Find an MST that contains exactly K White edges.
*/

//white edge-> weight 1
//black edge-> weight 0

/*
Thinking process:
1. We can use binary search on the number of white edges in the MST.
2. For a mid value in binary search, we modify the weights of the edges:
   - White edges get a weight of 1.
   - Black edges get a weight of 0.
3. We then run Kruskal's algorithm to find the MST with these modified weights.
4. If the number of white edges in the MST is less than K, we need to increase the weight of white edges to encourage their inclusion.
5. If the number of white edges is more than K, we decrease the weight of white edges.
6. We repeat this process until we find the MST with exactly K white edges or determine that it's not possible.
*/

#include <bits/stdc++.h>

using namespace std;

int findParent(int u, vector<int> &parent)
{
    if (parent[u] == u)
        return u;
    return parent[u] = findParent(parent[u], parent);
}

void unionSets(int u, int v, vector<int> &parent, vector<int> &rank)// whats rank?-> 
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

pair<int, int> krushkal(int n, vector<tuple<int, int, int>> &edges)
{
    // sorting edges on weight
    sort(edges.begin(), edges.end());

    vector<int> parent(n);
    vector<int> rank(n, 0);

    // parent array
    for (int i = 0; i < n; i++)
        parent[i] = i;

    int totalWeight = 0;
    int whiteEdgeCount = 0;

    for (auto &edge : edges)
    {
        int weight = get<0>(edge);
        int u = get<1>(edge);
        int v = get<2>(edge);

        int pu = findParent(u, parent);
        int pv = findParent(v, parent);

        if (pu != pv)
        {
            unionSets(u, v, parent, rank);
            totalWeight += weight;
            if (weight == 1)
                whiteEdgeCount++;
        }
    }

    return {totalWeight, whiteEdgeCount};
}