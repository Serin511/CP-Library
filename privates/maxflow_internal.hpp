#ifndef PRIVATES_MAXFLOW_INTERNAL
#define PRIVATES_MAXFLOW_INTERNAL 1
#include <bits/stdc++.h>
using namespace std;
struct Maxflow_Internal {
    using ll = long long;
    struct edge { int v, c, i; };
    Maxflow_Internal(int n) : n(n), adj(n) {}

public:
    void add_edge(int u, int v, int c) {
        if (u==v) return ;
        adj[u].push_back({v, c, (int)adj[v].size()});
        adj[v].push_back({u, 0, (int)adj[u].size()-1});
    }

    vector<bool> mincut(int s) {
        vector<bool> vis(n);
        queue<int> q;
        q.push(s);
        vis[s]=1;
        while (q.size()) {
            int u=q.front();
            q.pop();
            for (auto &e:adj[u]) if (e.c&&!vis[e.v]) {
                q.push(e.v);
                vis[e.c]=1;
            }
        }
        return vis;
    }

protected:
    int n;
    vector<vector<edge>> adj;
};
#endif // PRIVATES_MAXFLOW_INTERNAL