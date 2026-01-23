#include <bits/stdc++.h>


using namespace std;

struct Edge
{
    int u, v;
    double w;
};

bool arbitrage(int n, vector<vector<double>> &rate)
{

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (rate[i][k] == 0.0) continue;
            for (int j = 0; j < n; j++) {
                if (rate[k][j] == 0.0) continue;
                double candidate = rate[i][k] * rate[k][j];
                if (candidate > rate[i][j]) {
                    rate[i][j] = candidate;
                }
            }
        }
    }


    for (int i = 0; i < n; i++) {
        if (rate[i][i] > 1.0000001)
            return true;
    }

    return false;
}

int main()
{
    int n;
    cin >> n;

    map<string, int> id;

    for (int i = 0; i < n; i++)
    {
        string name;
        cin >> name;
        id[name] = i;
    }

    int m;
    cin >> m;

    vector<vector<double>> rate(n, vector<double>(n, 0.0));

    for(int i=0; i<n; i++){
        rate[i][i] = 1.0;
    }   
    
    for (int i = 0; i < m; i++)
    {
        string a, b;
        double r;
        cin >> a >> r >> b;

        int u = id[a];
        int v = id[b];


        if (r > rate[u][v])
            rate[u][v] = r;
    }

    if(arbitrage(n, rate)){
        cout << "Yes";
    }

    else
        cout << "No";

    return 0;
}
