#ifndef PRIVATES_MAXFLOW_HLPP
#define PRIVATES_MAXFLOW_HLPP
#include "privates/maxflow_internal.hpp"
#include <bits/stdc++.h>
using namespace std;
struct Maxflow : Maxflow_Internal {
    Maxflow(int n) : cnt(0), in(n), out(n), height(n), qu(2*n), Maxflow_Internal(n) {}

public:
    ll flow(int s, int t) {
        height[s]=n;
        out[s]=out[t]=LLONG_MAX;

        for (auto &e:adj[s]) if (e.c) {
            in[e.v]+=e.c;
            adj[e.v][e.i].c+=e.c;
            e.c=0;
            inqueue(e.v);
        }

        for (int hi=0; ;) {
            for (; qu[hi].empty(); hi--) if (hi==0) return in[t];
            hi=send(hi);
            if (cnt>=4*n) hi=max(hi, global(t));
        }
    }

private:
    int cnt;
    vector<vector<int>> qu;
    vector<ll> in, out;
    vector<int> height;

    void inqueue(int u) {
        qu[height[u]].emplace_back(u);
    }

    int outqueue(int h) {
        int u=qu[h].back();
        qu[h].pop_back();
        return u;
    }

    void push(int u, int v, int i) {
        auto go=min<ll>(in[u]-out[u], adj[u][i].c);
        if (in[v]==out[v]) inqueue(v);
        in[v]+=go;
        out[u]+=go;
        adj[v][adj[u][i].i].c+=go;
        adj[u][i].c-=go;
    }

    int send(int h) {
        int u=outqueue(h), mn=2*n;
        if (height[u]!=h) return h;

        for (auto &e:adj[u]) if (in[u]>out[u]&&e.c) {
            if (height[u]==height[e.v]+1) push(u, e.v, adj[e.v][e.i].i);
            else mn=min(mn, height[e.v]+1);
        }

        if (in[u]>out[u]) {
            height[u]=mn;
            cnt++;
            if (mn<n) { inqueue(u); return mn; }
        }

        return h;
    }

    int global(int t) {
        cnt=0;
        for (auto &i:qu) i.clear();
        ranges::fill(height, 2*n+1);

        queue<int> q;
        q.emplace(t);
        height[t]=0;

        int mx=0;

        while (q.size()) {
            int u=q.front();
            q.pop();
            int h=height[u]+1;

            for (auto &e:adj[u]) {
                if (adj[e.v][e.i].c&&h<height[e.v]) {
                    q.emplace(e.v);
                    height[e.v]=h;
                    if (in[e.v]>out[e.v]) inqueue(e.v), mx=max(mx, h);
                }
            }
        }

        return mx;
    }
};
#endif // PRIVATES_MAXFLOW_HLPP