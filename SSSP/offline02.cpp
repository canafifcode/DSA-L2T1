#include<bits/stdc++.h>

using namespace std;

void find_neg_cycle(int n, vector<tuple<int,int,int>> &edges){
    
    vector<long long> dist(n+1, 0);
    vector<int> parent(n+1, -1);
    
    dist[1]=0;
    int start_node=-1;

    for (int i = 1; i <= n; ++i) {
        start_node = -1;
        bool flag_update = false;
        for (const auto& edge : edges) {
            int u, v, w;
            tie(u, v, w) = edge; 

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u; // Recording rasta
                flag_update = true;
                if (i == n) {
                    start_node = v; 
                }
            }
        }
        if(i<n && !flag_update){
            break; 
        }
        if(i==n && flag_update){
            break; // negative cycle detected
        }
    }

    if(start_node==-1){
        cout<<"-1\n";
        return;
    }

    for (int i = 0; i < n; ++i) {
        start_node = parent[start_node];
    }

    int curr_node = start_node;
    deque<int> cycle_path;
    int temp_node= curr_node;
    do{
        cycle_path.push_front(temp_node);
        temp_node = parent[temp_node];
    }while(temp_node != curr_node);
    
    //cycle_path.push_front(curr_node); 

    //cout<< "YES\n";

    for(int node : cycle_path){
        cout<<node<<" ";
    }

}

int main(){
    int m,n;
    cin>>n>>m;
    vector<tuple<int,int,int>> edges;

    for(int i=0;i<m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        edges.push_back({u,v,w});
    }

    find_neg_cycle(n,edges);
}
