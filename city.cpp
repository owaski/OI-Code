#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
 
#define ll long long
 
template<class T>
inline void chkmax(T &x, T y) {
    if(x < y) x = y;
}
 
template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}
int outn;
char out[10000000];
template<class T>
inline void write(T x) {
    if(x < 0) out[outn++] = '-', x = -x;
    if(x) {
        int tmpn = 0;char tmp[20];
        while(x) tmp[tmpn++] = x%10+'0', x /= 10;
        while(tmpn) out[outn++] = tmp[--tmpn];
    }
    else out[outn++] = '0';
}
 
const int N = 20000;
const int M = 50000;
const int Q = 50000;
const int LOG = 20;
const int INF = 1e9;
 
int n, q;
 
struct edge {
    int u, v, w, id;
    edge() {u = v = w = id = 0;}
    edge(int _u, int _v, int _w, int _id):u(_u), v(_v), w(_w), id(_id) {}
    inline void input() {
        read(u), read(v), read(w);
    }
    inline friend bool operator < (const edge &p, const edge &q) {
        return p.w < q.w;
    }
}e[LOG][M+9], g[M+9], tmp[M+9];
int w[M+9];
int en[LOG];
int pos[M+9];
 
struct operation {
    int k, d;
    operation() {k = d = 0;}
    operation(int _k, int _d):k(_k), d(_d) {}
    inline void input() {
        read(k), read(d);
    }
}op[Q+9];
 
int fa[N+9], rk[N+9];
 
ll ans[Q+9];
 
inline void init() {
    read(n), read(en[0]), read(q);
    for(int i = 1; i <= en[0]; ++i)
        e[0][i].input(), e[0][i].id = i, w[i] = e[0][i].w;
    for(int i = 1; i <= q; ++i)
        op[i].input();
}
 
inline void reset(int m, edge *g) {
    for(int i = 1; i <= m; ++i) {
        edge now = g[i];
        fa[now.u] = now.u, fa[now.v] = now.v;
        rk[now.u] = rk[now.v] = 1;
    }
}
 
inline int find(int x) {
    int anc = x, y;
    while(anc != fa[anc]) anc = fa[anc];
    while(x != anc) y = fa[x], fa[x] = anc, x = y;
    return anc;
}
 
inline bool merge(int u, int v) {
    u = find(u), v = find(v);
    if(u == v) return false;
    if(rk[u] > rk[v]) std::swap(u, v);
    fa[u] = v, chkmax(rk[v], rk[u]+1);
    return true;
}
 
inline void contraction(int l, int r, int &m, ll &cnt) {
    int tmpn = 0;
    reset(m, g);
    std::sort(g+1, g+m+1);
    for(int i = 1; i <= m; ++i)
        if(merge(g[i].u, g[i].v) || g[i].w == -INF)
            tmp[++tmpn] = g[i];
    reset(tmpn, tmp);
    for(int i = 1; i <= tmpn; ++i)
        if(tmp[i].w != -INF) {
            cnt += tmp[i].w;
            merge(tmp[i].u, tmp[i].v);
        }
    tmpn = 0;
    for(int i = 1; i <= m; ++i)
        if(find(g[i].u) != find(g[i].v)) {
            tmp[++tmpn] = g[i];
            tmp[tmpn].u = find(g[i].u);
            tmp[tmpn].v = find(g[i].v);
        }
    for(int i = l; i <= r; ++i)
        pos[op[i].k] = 0;
    for(int i = 1; i <= tmpn; ++i)
        g[i] = tmp[i], pos[g[i].id] = i;
    m = tmpn;
}
 
inline void reduction(int &m, int d) {
    int tmpn = 0;
    reset(m, g);
    std::sort(g+1, g+m+1);
    for(int i = 1; i <= m; ++i)
        if(merge(g[i].u, g[i].v) || g[i].w == INF)
            tmp[++tmpn] = g[i];
    en[d+1] = tmpn;
    for(int i = 1; i <= tmpn; ++i)
        e[d+1][i] = tmp[i];
}
 
void dac(int l, int r, int d, ll cnt) {
    int m = en[d];
    for(int i = l; i <= r; ++i)
        pos[op[i].k] = 0;
    if(l == r) w[op[l].k] = op[l].d;
    for(int i = 1; i <= m; ++i) {
        e[d][i].w = w[e[d][i].id];
        g[i] = e[d][i], pos[g[i].id] = i;
    }
    if(l == r) {
        ans[l] = cnt;
        reset(m, g);
        std::sort(g+1, g+m+1);
        for(int i = 1; i <= m; ++i)
            if(merge(g[i].u, g[i].v))
                ans[l] += g[i].w;
        return ;
    }
    for(int i = l; i <= r; ++i)
        g[pos[op[i].k]].w = -INF;
    contraction(l, r, m, cnt);
    for(int i = l; i <= r; ++i)
        g[pos[op[i].k]].w = INF;
    reduction(m, d);
    int mid = (l+r)>>1;
    dac(l, mid, d+1, cnt);
    dac(mid+1, r, d+1, cnt);
}
 
inline void solve() {
    dac(1, q, 0, 0);
    for(int i = 1; i <= q; ++i)
        write(ans[i]), out[outn++] = '\n';
    out[--outn] = 0, puts(out);
}
 
int main() {
    init();
    solve();
    return 0;
}
