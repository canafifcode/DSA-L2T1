#include <bits/stdc++.h>
using namespace std;

const long long INF = (long long)4e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    // dist[i][j][0] → no free edge used
    // dist[i][j][1] → free edge already used
    vector<vector<vector<long long>>> dist(
        n, vector<vector<long long>>(n, vector<long long>(2, INF))
    );

    for (int i = 0; i < n; i++) {
        dist[i][i][0] = 0;
        dist[i][i][1] = 0;
    }

    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        dist[u][v][0] = min(dist[u][v][0], w);
        dist[u][v][1] = min(dist[u][v][1], w); // normal edge
    }

    // Modified Floyd–Warshall
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // no free edge used
                dist[i][j][0] = min(
                    dist[i][j][0],
                    dist[i][k][0] + dist[k][j][0]
                );

                // free edge already used
                dist[i][j][1] = min(
                    dist[i][j][1],
                    dist[i][k][1] + dist[k][j][1]
                );

                // use free edge now
                dist[i][j][1] = min(
                    dist[i][j][1],
                    dist[i][k][0] + dist[k][j][0]
                );
            }
        }
    }

    int q;
    cin >> q;
    while (q--) {
        int u, v;
        cin >> u >> v;
        long long ans = min(dist[u][v][0], dist[u][v][1]);
        if (ans >= INF) cout << -1 << "\n";
        else cout << ans << "\n";
    }

    return 0;
}
