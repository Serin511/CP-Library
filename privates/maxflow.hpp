#ifndef PRIVATES_MAXFLOW
#define PRIVATES_MAXFLOW 1
#include "privates/maxflow_internal.hpp"
#include <bits/stdc++.h>
using namespace std;
struct Maxflow : Maxflow_Internal {
    Maxflow(int n) : lst(n), dist(n), Maxflow_Internal(n) {}

public:
    long long flow(int s, int t) {
        if (s==t) return 0;
        long long rt=0;
        auto bfs = [&](int b) {
            ranges::fill(dist, -1);
            queue<int> q;
            q.push(s);
            dist[s]=0;
            while (q.size()) {
                int u=q.front();
                q.pop();
                for (auto &e:adj[u]) if (e.c>=b&&!~dist[e.v])
                    q.push(e.v), dist[e.v]=dist[u]+1;
            }
            return ~dist[t];
        };
        for (int b=(1<<30); b; b/=2) while (bfs(b)) {
            ranges::fill(lst, 0);
            for (; dfs(s, t, b); rt+=b);
        }
        return rt;
    }

private:
    vector<int> lst, dist;

    bool dfs(int u, int v, int mn) {
        if (u==v) return true;
        for (int &t=lst[u]; t<adj[u].size(); t++) {
            auto &e=adj[u][t];
            if (dist[u]+1==dist[e.v]&&e.c>=mn&&dfs(e.v, v, mn)) {
                e.c-=mn;
                adj[e.v][e.i].c+=mn;
                return true;
            }
        }
        return false;
    }
};
#endif // PRIVATES_MAXFLOW