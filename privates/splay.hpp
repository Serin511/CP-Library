#ifndef PRIVATES_SPLAY
#define PRIVATES_SPLAY 1
#include <bits/stdc++.h>
using namespace std;
template <class T, class L, auto op, auto mapping, auto push, T tid, L lid>
struct splay_tree {
	struct node {
		int ch[2], p, sz;
		bool rev;
		T val, agg, ragg;
		L lz;
	};
	static constexpr node nil={{0,0}, 0, 0, false, tid, tid, tid, lid};
	inline static vector<node> nd={nil};
	inline static int pp=0;
	int root=0;

	static void init(int n) {
		pp=0;
		nd.assign(n+1, nil);
	}
	splay_tree() {}
	explicit splay_tree(const vector<T> &v) {
		if (!v.empty()) {
			nd.resize(v.size()+1);
			root=build(v, 0, (int)v.size()-1);
		}
	}

	int size() { return nd[root].sz; }

	void insert(int pos, T val) {
		int l, r;
		split_(pos, l, r);
		int x=new_node(val);
		root=merge_(merge_(l, x), r);
	}
	void erase(int pos) {
		kth_(pos);
		int l=nd[root].ch[0], r=nd[root].ch[1];
		nd[l].p=nd[r].p=0;
		root=merge_(l, r);
	}
	void push_back(T val) { insert(size(), val); }
	void push_front(T val) { insert(0, val); }

	T get(int pos) {
		kth_(pos);
		return nd[root].val;
	}
	T get(int l, int r) {
		int a, b, c;
		split_(l, a, b);
		root=b; split_(r-l+1, b, c);
		T res=nd[b].agg;
		root=merge_(a, merge_(b, c));
		return res;
	}
	T get_all() { return nd[root].agg; }

	void set(int pos, T val) {
		kth_(pos);
		nd[root].val=val;
		pull(root);
	}
	void upd(int l, int r, L v) {
		int a, b, c;
		split_(l, a, b);
		root=b; split_(r-l+1, b, c);
		apply_(b, v);
		root=merge_(a, merge_(b, c));
	}

	void reverse(int l, int r) {
		int a, b, c;
		split_(l, a, b);
		root=b; split_(r-l+1, b, c);
		flip(b);
		root=merge_(a, merge_(b, c));
	}
	void shift(int l, int r, int k) {
		int len=r-l+1;
		k=((k%len)+len)%len;
		if (!k) return;
		int a, b, c;
		split_(l, a, b);
		root=b; split_(len, b, c);
		int b1, b2;
		root=b; split_(len-k, b1, b2);
		root=merge_(a, merge_(merge_(b2, b1), c));
	}

	template <class G>
	int lower_bound(G g) {
		int x=root, pos=0, last=0;
		while (x) {
			last=x; prop(x);
			if (g(nd[x].val)) {
				pos+=nd[nd[x].ch[0]].sz+1;
				x=nd[x].ch[1];
			} else x=nd[x].ch[0];
		}
		if (last) { splay(last); root=last; }
		return pos;
	}

	template <class G>
	int max_right(int l, G g) {
		if (l>=size()) return size();
		int a, b;
		split_(l, a, b);
		root=b;
		int k=mr_(root, g);
		root=merge_(a, b);
		return l+k;
	}
	template <class G>
	int min_left(int r, G g) {
		if (r<0) return -1;
		int a, b;
		split_(r+1, a, b);
		root=a;
		int k=ml_(root, g);
		root=merge_(a, b);
		return r-k;
	}

	splay_tree split(int k) {
		splay_tree ret;
		int l, r;
		split_(k, l, r);
		root=l;
		ret.root=r;
		return ret;
	}
	void merge(splay_tree &o) {
		root=merge_(root, o.root);
		o.root=0;
	}

	vector<T> to_vec() {
		vector<T> res;
		res.reserve(size());
		dfs_(root, res);
		return res;
	}

private:
	static int new_node(T v) {
		int x=++pp;
		if (x>=(int)nd.size()) nd.resize(x*2, nil);
		nd[x]={{0,0}, 0, 1, false, v, v, v, lid};
		return x;
	}

	static void pull(int x) {
		if (!x) return;
		nd[x].sz=nd[nd[x].ch[0]].sz+1+nd[nd[x].ch[1]].sz;
		nd[x].agg=op(op(nd[nd[x].ch[0]].agg, nd[x].val), nd[nd[x].ch[1]].agg);
		nd[x].ragg=op(op(nd[nd[x].ch[1]].ragg, nd[x].val), nd[nd[x].ch[0]].ragg);
	}

	static void apply_(int x, L l) {
		if (!x) return;
		mapping(l, nd[x].val);
		mapping(l, nd[x].agg);
		mapping(l, nd[x].ragg);
		nd[x].lz=push(nd[x].lz, l);
	}

	static void flip(int x) {
		if (!x) return;
		nd[x].rev^=1;
		swap(nd[x].agg, nd[x].ragg);
	}

	static void prop(int x) {
		if (!x) return;
		if (nd[x].rev) {
			swap(nd[x].ch[0], nd[x].ch[1]);
			flip(nd[x].ch[0]);
			flip(nd[x].ch[1]);
			nd[x].rev=false;
		}
		apply_(nd[x].ch[0], nd[x].lz);
		apply_(nd[x].ch[1], nd[x].lz);
		nd[x].lz=lid;
	}

	static int dir(int x) { return nd[nd[x].p].ch[1]==x; }

	static void rot(int x) {
		int y=nd[x].p, z=nd[y].p, d=dir(x);
		nd[y].ch[d]=nd[x].ch[d^1];
		if (nd[x].ch[d^1]) nd[nd[x].ch[d^1]].p=y;
		nd[x].ch[d^1]=y;
		nd[y].p=x;
		nd[x].p=z;
		if (z) nd[z].ch[nd[z].ch[1]==y]=x;
		pull(y);
		pull(x);
	}

	static void splay(int x) {
		while (nd[x].p) {
			int y=nd[x].p, z=nd[y].p;
			if (z) (dir(x)==dir(y) ? rot(y) : rot(x));
			rot(x);
		}
	}

	void kth_(int k) {
		int x=root;
		while (true) {
			prop(x);
			int ls=nd[nd[x].ch[0]].sz;
			if (k<ls) x=nd[x].ch[0];
			else if (k==ls) break;
			else { k-=ls+1; x=nd[x].ch[1]; }
		}
		splay(x);
		root=x;
	}

	static int build(const vector<T> &v, int l, int r) {
		if (l>r) return 0;
		int m=(l+r)/2;
		int x=new_node(v[m]);
		nd[x].ch[0]=build(v, l, m-1);
		nd[x].ch[1]=build(v, m+1, r);
		if (nd[x].ch[0]) nd[nd[x].ch[0]].p=x;
		if (nd[x].ch[1]) nd[nd[x].ch[1]].p=x;
		pull(x);
		return x;
	}

	void split_(int k, int &l, int &r) {
		if (k<=0) { l=0; r=root; root=0; return; }
		if (k>=nd[root].sz) { l=root; r=0; root=0; return; }
		kth_(k-1);
		r=nd[root].ch[1];
		nd[root].ch[1]=0;
		if (r) nd[r].p=0;
		pull(root);
		l=root;
		root=0;
	}

	int merge_(int l, int r) {
		if (!l) return r;
		if (!r) return l;
		root=l;
		int x=l;
		prop(x);
		while (nd[x].ch[1]) { x=nd[x].ch[1]; prop(x); }
		splay(x); root=x;
		nd[x].ch[1]=r;
		nd[r].p=x;
		pull(x);
		return x;
	}

	template <class G>
	static int mr_(int x, G &g) {
		T acc=tid;
		int res=0;
		while (x) {
			prop(x);
			T wl=op(acc, nd[nd[x].ch[0]].agg);
			if (!g(wl)) { x=nd[x].ch[0]; continue; }
			T wn=op(wl, nd[x].val);
			if (!g(wn)) { res+=nd[nd[x].ch[0]].sz; break; }
			acc=wn;
			res+=nd[nd[x].ch[0]].sz+1;
			x=nd[x].ch[1];
		}
		return res;
	}

	template <class G>
	static int ml_(int x, G &g) {
		T acc=tid;
		int res=0;
		while (x) {
			prop(x);
			T wr=op(nd[nd[x].ch[1]].agg, acc);
			if (!g(wr)) { x=nd[x].ch[1]; continue; }
			T wn=op(nd[x].val, wr);
			if (!g(wn)) { res+=nd[nd[x].ch[1]].sz; break; }
			acc=wn;
			res+=nd[nd[x].ch[1]].sz+1;
			x=nd[x].ch[0];
		}
		return res;
	}

	static void dfs_(int x, vector<T> &res) {
		if (!x) return;
		prop(x);
		dfs_(nd[x].ch[0], res);
		res.push_back(nd[x].val);
		dfs_(nd[x].ch[1], res);
	}
};
#endif // PRIVATES_SPLAY
