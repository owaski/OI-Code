#include "oath.h"
#include <bits/stdc++.h>

#define ll long long

#define debug(...) fprintf(stderr, __VA_ARGS__)

#define pii std::pair<int,int>
#define mp std::make_pair
#define SZ(x) (int)(x).size()
#define pb push_back

template<class T>inline void chkmax(T &x, const T &y) {if(x < y) x = y;}
template<class T>inline void chkmin(T &x, const T &y) {if(x > y) x = y;}

const int N = 300000;
const int S = 30000000;
const int LOG = 20;

int getrand() {
    return (rand() << 15) | rand();
}

struct node {
    node *c[2];
    Data *pre, suf;
    int pos, key;
    void init(int p) {
        c[0] = c[1] = 0;
        pre = 0, suf = Data(0);
        pos = p, key = getrand();
    }
}p[S + 9];
int pn;

node *rt[N + 9];
int sz[N + 9];
int n;
int f[N + 9][LOG];

void Init() {
    pn = 0, rt[0] = 0, sz[0] = 0, n = 0;
}

Data st[N + 9];
int stn;

void dfs(node *x) {
    if(x->c[1]) dfs(x->c[1]);
    st[++stn] = x->pre[0];
    if(x->c[0]) dfs(x->c[0]);
}

void build(node *x, Data v) {
    stn = 0;
    if(x->c[0]) dfs(x->c[0]);
    x->pre = new Data[stn + 1];
    x->pre[0] = v;
    for(int i = 1; i <= stn; ++i)
        x->pre[i] = F(st[i], x->pre[i - 1]);
    x->suf = Data(0);
}

void ins(node *&x, int pos, Data v, bool isroot) {
    if(!x) {
        x = p + (pn++), x->init(pos);
        // debug("%d\n", pn);
        if(isroot) build(x, v);
        return ;
    }
    p[pn] = *x, x = p + pn, pn++;
    // debug("%d\n", pn);
    ins(x->c[1], pos, v, false);
    if(x->c[1]->key < x->key) {
        node *y = x->c[1];
        x->c[1] = y->c[0], y->c[0] = x, x = y;
        if(isroot) build(x, v);
    }
    else {
        if(x->c[1]->pos == pos) build(x->c[1], v);
        if(!x->suf.x) x->suf = v;
        else x->suf = F(x->suf, v);
    }
}

void Add_element(int id, Data x) {
    rt[++n] = rt[id], sz[n] = sz[id] + 1, f[n][0] = id;
    for(int i = 1; i < LOG; ++i) f[n][i] = f[f[n][i - 1]][i - 1];
    ins(rt[n], sz[n], x, true);
}

void Del_element(int id) {
    ++n;
    rt[n] = rt[f[id][0]], sz[n] = sz[f[id][0]];
    memcpy(f[n], f[f[id][0]], sizeof f[n]);
}

Data qry(node *x, int l) {
    if(x->pos < l) return qry(x->c[1], l);
    else {
        if(x->suf.x) return F(x->pre[x->pos - l], x->suf);
        else return x->pre[x->pos - l];
    }
}

Data Query_range(int id, int l, int r) {
    int d = sz[id] - r;
    for(int i = LOG - 1; i >= 0; --i)
        if(d >> i & 1) id = f[id][i];
    return qry(rt[id], l);
}
