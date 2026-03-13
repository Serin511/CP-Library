#ifndef PRIVATES_SEGBEATS
#define PRIVATES_SEGBEATS 1
#include <bits/stdc++.h>
using namespace std;
template <class T, class L, auto op, auto try_apply, auto push, T tid, L lid>
struct segtree_beats {
    explicit segtree_beats(int n=0) : segtree_beats(vector<T>(n, tid)) {}
    explicit segtree_beats(const vector<T> &v): n(v.size()) {
        b=bit_ceil((unsigned)v.size());
        log_=countr_zero((unsigned)b);
        st.resize(2*b, tid);
        lz.resize(b, lid);
        for (int i=0; i<n; i++) st[i+b]=v[i];
        for (int i=b; --i>0; ) up(i);
    }

public:
    void set(int t, T v) {
        t+=b;
        for (int i=log_; i>0; i--) down(t>>i);
        st[t]=v;
        for (int i=1; i<=log_; i++) up(t>>i);
    }

    void upd(int t, L v) {
        t+=b;
        for (int i=log_; i>0; i--) down(t>>i);
        try_apply(v, st[t], st[t]);
        for (int i=1; i<=log_; i++) up(t>>i);
    }
    void upd(int l, int r, L v) {
        l=max(l, 0);
        r=min(r, n-1);
        if (l>r) return ;

        l+=b, r+=b;
        for (int i=log_; i>0; i--) {
            if (((l>>i)<<i)!=l) down(l>>i);
            if (((r+1>>i)<<i)!=r+1) down(r>>i);
        }

        for (int i=l, j=r; i<=j; i/=2, j/=2) {
            if (i&1) down_(i++, v);
            if (~j&1) down_(j--, v);
        }

        for (int i=1; i<=log_; i++) {
            if (((l>>i)<<i)!=l) up(l>>i);
            if (((r+1>>i)<<i)!=r+1) up(r>>i);
        }
    }

    T get(int t) {
        t+=b;
        for (int i=log_; i>0; i--) down(t>>i);
        return st[t];
    }
    T get(int l, int r) {
        l=max(l, 0);
        r=min(r, n-1);
        if (l>r) return tid;

        l+=b, r+=b;
        for (int i=log_; i>0; i--) {
            if (((l>>i)<<i)!=l) down(l>>i);
            if (((r+1>>i)<<i)!=r+1) down(r>>i);
        }

        T lv{}, rv{};
        for (; l<=r; l/=2, r/=2) {
            if (l&1) lv=op(lv, st[l++]);
            if (~r&1) rv=op(st[r--], rv);
        }
        return op(lv, rv);
    }

    template <bool (*g)(T)> int max_right(int l) {
        return max_right(l, [](T x) { return g(x); });
    }
    template <class G> int max_right(int l, G g) {
        if (l>=n) return n;
        l+=b;
        for (int i=log_; i>0; i--) down(l>>i);
        T sm=tid;
        do {
            while (~l&1) l/=2;
            if (!g(op(sm, st[l]))) {
                while (l<b) {
                    down(l);
                    l=2*l;
                    if (g(op(sm, st[l]))) {
                        sm=op(sm, st[l]);
                        l++;
                    }
                }
                return l-b;
            }
            sm=op(sm, st[l]);
            l++;
        } while ((l&-l)!=l);
        return n;
    }

    template <bool (*g)(T)> int min_left(int r) {
        return min_left(r, [](T x) { return g(x); });
    }
    template <class G> int min_left(int r, G g) {
        if (r<0) return -1;
        r+=b;
        for (int i=log_; i>0; i--) down(r>>i);
        T sm=tid;
        r++;
        do {
            r--;
            while (r>1&&(r&1)) r>>=1;
            if (!g(op(st[r], sm))) {
                while (r<b) {
                    down(r);
                    r=2*r+1;
                    if (g(op(st[r], sm))) {
                        sm=op(st[r], sm);
                        r--;
                    }
                }
                return r-b;
            }
            sm=op(st[r], sm);
        } while ((r&-r)!=r);
        return -1;
    }

    T get_all() { return st[1]; }

private:
    int n, b, log_;
    vector<T> st;
    vector<L> lz;

    void up(int i) { st[i]=op(st[i*2], st[i*2+1]); }
    void down_(int i, L l) {
        bool res=try_apply(l, st[i], st[i]);
        if (i<b) {
            lz[i]=push(lz[i], l);
            if (!res) down(i), up(i);
        }
    }
    void down(int i) {
        down_(2*i, lz[i]);
        down_(2*i+1, lz[i]);
        lz[i]=lid;
    }
};
#endif // PRIVATES_SEGBEATS