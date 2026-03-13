#ifndef PRIVATES_DISJOINT_SET
#define PRIVATES_DISJOINT_SET 1
#include <bits/stdc++.h>
using namespace std;
struct djs {
	vector<int> p;
	djs(int n) : p(n+1) {}
	int gp(int u) { return p[u]?(p[u]=gp(p[u])):u; }
	bool un(int u, int v) {
		u=gp(u), v=gp(v);
		if (u!=v) p[v]=u;
		return u!=v;
	}
};
#endif // PRIVATES_DISJOINT_SET