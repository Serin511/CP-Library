#ifndef PRIVATES_CIRCULATION
#define PRIVATES_CIRCULATION 1
#include "privates/maxflow"
#include <bits/stdc++.h>
using namespace std;
struct Circulation {
    Circulation(int n) : flow(n+2) {}

public:
    void add_edge(int u, int v, int l, int r) {
        flow.add_edge(0, v+2, l);
        flow.add_edge(u+2, 1, l);
        flow.add_edge(u+2, v+2, r-l);
        sum+=l;
    }
    
    bool solve(int s, int t) {
        flow.add_edge(t, s, INT_MAX);
        return sum==flow.flow(0, 1);
    }

private:
    Maxflow flow;
    long long sum{};
};
#endif // PRIVATES_CIRCULATION