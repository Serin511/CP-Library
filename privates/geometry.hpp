#ifndef PRIVATES_GEOMETRY
#define PRIVATES_GEOMETRY 1
#include <bits/stdc++.h>
using namespace std;
bool angle_cmp(auto x, auto y) {
	if ((x.imag()>=0)!=(y.imag()>=0)) return x.imag()>y.imag();
	auto cr=(conj(x)*y).imag();
	if (cr==0) return make_pair(x.real(), x.imag())>make_pair(y.real(), y.imag());
	return cr>0;
}

auto dot(auto x, auto y) {
    return (conj(x)*y).real();
}

auto cross(auto x, auto y) {
    return (conj(x)*y).imag();
}

auto ccw(auto x, auto y, auto z) {
    return cross(y-x, z-x);
}

auto convex_hull(auto pnts) {
    ranges::sort(pnts, [](auto x, auto y) { return make_pair(x.real(), x.imag())<make_pair(y.real(), y.imag()); });

    decltype(pnts) l, r;
    for (auto p:pnts) {
        while (l.size()>1&&ccw(l[l.size()-2], l.back(), p)<=0) l.pop_back();
        l.push_back(p);
    }
    ranges::reverse(pnts);
    for (auto p:pnts) {
        while (r.size()>1&&ccw(r[r.size()-2], r.back(), p)<=0) r.pop_back();
        r.push_back(p);
    }

    l.pop_back();
    r.pop_back();
    l.insert(l.end(), r.begin(), r.end());
    return l;
}
#endif // PRIVATES_GEOMETRY