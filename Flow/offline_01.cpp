//ford fulkerson using edmonds karp algorithm
//use set to store the graph
#include <bits/stdc++.h>

using namespace std;

//function to find bfs path from s to t
bool bfs(vector<vector<pair<int,int>>>& graph, int s, int t, vector<int>& parent){
    int n = graph.size();
    vector<bool> visited(n,false);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while(!q.empty()){
        int u = q.front();
        q.pop();

        for(auto &edge: graph[u]){
            int v = edge.first;
            int cap = edge.second;

            if(!visited[v] && cap > 0){ //not visited and has capacity
                q.push(v);
                parent[v] = u;
                visited[v] = true;

                if(v == t){
                    return true;
                }
            }
        }
    }
    return false; //no path found
}


int maxFlow(vector<vector<pair<int,int>>>& graph, int s, int t){
    int total_Flow = 0;
    int n = graph.size();
    vector<int> parent(n);

    while (bfs(graph, s, t, parent))
    {
        int path_Flow = INT_MAX;
        for(int v = t; v != s; v = parent[v]){
            int u = parent[v];
            //find the capacity of edge u->v
            for(auto &edge: graph[u]){
                if(edge.first == v){
                    path_Flow = min(path_Flow, edge.second);
                    break;
                }
            }
        }
        total_Flow += path_Flow;
       
        for(int v = t; v != s; v = parent[v]){
            int u = parent[v];
            for(auto &edge: graph[u]){
                if(edge.first == v){
                    edge.second -= path_Flow;
                    break;
                }
            }
            for(auto &edge: graph[v]){
                if(edge.first == u){
                    edge.second += path_Flow;
                    break;
                }
            }
        }
    }
    return total_Flow;
    
}

int main(){
    int n ,m;
    cin>>n>>m;
    
    vector<vector<pair<int,int>>> graph(n);
    vector<vector<pair<int,int>>> original(n);
    for(int i=0;i<m;i++){
        int u,v,cap;
        cin>>u>>v>>cap;
        graph[u].push_back({v,cap});
        original[u].push_back({v,cap});
        graph[v].push_back({u,0}); //rev edg 0 cap
        original[v].push_back({u,0});
    }

    

    int s, t;
    cin>>s>>t;

    int max_Flow = maxFlow(graph, s, t);

    cout<<max_Flow<<endl;

    //print the flow graph
    
    for (int u = 0; u < n; u++) {
    for (auto &edge : original[u]) {
        int v = edge.first;
        int cap = edge.second;

        // ONLY forward edges
        if (cap == 0) continue;

        int flow = 0;
        for (auto &e : graph[v]) {
            if (e.first == u) {
                flow = e.second;
                break;
            }
        }

     
        cout << u << " " << v << " " << flow << "/" << cap << "\n";
    
    }
}

}