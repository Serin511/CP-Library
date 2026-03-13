#ifndef PRIVATES_MCMF
#define PRIVATES_MCMF 1
#include <bits/stdc++.h>
using namespace std;
struct MCMF {
    using ll = long long;
    struct edge { int v, c, l, i; };
	
	MCMF(int n) : n(n), adj(n), dist(n), lst(n), in(n) {}

public:
	void add_edge(int u, int v, int c, int l) {
		adj[u].push_back({v, c, l, (int)adj[v].size()});
		adj[v].push_back({u, 0, -l, (int)adj[u].size()-1});
	}
    
	array<ll, 2> flow(int s, int t) {
		array<ll, 2> rt{0, 0};
		
        while (1) {
            spfa(s);
			if (!~lst[t]) break;

            ll fl=LLONG_MAX;
            ll cost=dfs(t, fl);
            rt[0]+=fl, rt[1]+=fl*cost;
		}

		return rt;
	}

private:
    int n;
    vector<vector<edge>> adj;
    vector<int> lst, in;
    vector<ll> dist;
    
    void spfa(int s) {
        ranges::fill(lst, -1);
        ranges::fill(dist, LLONG_MAX);
        ranges::fill(in, 0);

        deque<int> dq={s};
        dist[s]=0;
        in[s]=1;

        while (dq.size()) {
            int u=dq.front();
            dq.pop_front();
            in[u]=0;
            for (auto &e:adj[u]) if (e.c&&dist[u]+e.l<dist[e.v]) {
                dist[e.v]=dist[u]+e.l;
                lst[e.v]=u;
                if (!in[e.v]) {
                    in[e.v]=1;
                    if (dq.size()&&dist[e.v]<dist[dq.front()]) dq.push_front(e.v);
                    else dq.push_back(e.v);
                }
            }
        }
    }

    ll dfs(int u, ll &fl) {
        if (!~lst[u]) return 0;
        for (auto &e:adj[lst[u]]) if (e.v==u&&e.c&&dist[u]==dist[lst[u]]+e.l) {
            fl=min<ll>(fl, e.c);
            ll rt=dfs(lst[u], fl);
            e.c-=fl;
            adj[u][e.i].c+=fl;
            return rt+e.l;
        }
        assert(false);
    }
};
#endif // PRIVATES_MCMF