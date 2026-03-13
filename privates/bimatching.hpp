#ifndef PRIVATES_BIMATCHING
#define PRIVATES_BIMATCHING 1
#include <bits/stdc++.h>
using namespace std;
struct Bimatching {
    Bimatching(int n, int m) : n(n), m(m), adj(n), con(n, -1), rev(m, -1), dist(n), lst(n) {}

public:
    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    int matching() {
        int rt=0;

        while (1) {
            bfs();

            ranges::fill(lst, 0);
            int fl=0;

            for (int i=0; i<n; i++) if (!~con[i]) fl+=dfs(i);
            if (!fl) break;
            rt+=fl;
        }

        return rt;
    }

    vector<array<int, 2>> result() {
        vector<array<int, 2>> rt;
        for (int i=0; i<n; i++) if (~con[i]) rt.push_back({i, con[i]});
        return rt;
    }

private:
    vector<vector<int>> adj;
    vector<int> con, rev, dist, lst;
    int n, m;

    void bfs() {
        ranges::fill(dist, -1);
        queue<int> q;
        for (int i=0; i<n; i++) if (!~con[i]) q.push(i), dist[i]=0;
        while (q.size()) {
            int u=q.front();
            q.pop();
            for (int v:adj[u]) if (~rev[v]&&!~dist[rev[v]]) {
                dist[rev[v]]=dist[u]+1;
                q.push(rev[v]);
            }
        }
    }

    bool dfs(int u) {
        for (int &p=lst[u]; p<adj[u].size(); p++) {
            int v=adj[u][p];
            if (!~rev[v]||(dist[rev[v]]==dist[u]+1&&dfs(rev[v]))) {
                rev[v]=u;
                con[u]=v;
                return true;
            }
        }
        return false;
    }
};
#endif // PRIVATES_BIMATCHING