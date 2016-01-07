#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

const int N = 10009;
const int M = N*10;
const int LOG = 15;

int n, m, q;
struct Graph {
    int sz, head[N], to[M], w[M], ne[M];
    Graph() {
        sz = 1, memset(head, 0, sizeof head);
    }
    inline void addedge(int u, int v, int _w) {
        to[sz] = v, ne[sz] = head[u], w[sz] = _w, head[u] = sz++;
        to[sz] = u, ne[sz] = head[v], w[sz] = _w, head[v] = sz++;
    }
}cactus, tree;

int dfn[N];
int tot;
int low[N];
int fa[N];
int fw[N];

int ringn;
int len[N];
int bel[N];

int d[N][2];
int st[N];
int top;

int dis[N];
int dep[N];
int f[N][LOG];

inline void init() {
    scanf("%d%d%d", &n, &m, &q);
    for(int i = 1; i <= m; ++i) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        cactus.addedge(u, v, w);
    }
}

void dfs_cactus(int now) {
    low[now] = dfn[now] = ++tot;
    for(int i = cactus.head[now], to; i; i = cactus.ne[i])
        if(!dfn[to = cactus.to[i]]) {
            fa[to] = now, fw[to] = cactus.w[i], dfs_cactus(to);
            low[now] = std::min(low[now], low[to]);
        }
        else if(to != fa[now])
            low[now] = std::min(low[now], dfn[to]);
    for(int i = cactus.head[now], to; i; i = cactus.ne[i])
        if(fa[to = cactus.to[i]] == now && low[to] > dfn[now])
            tree.addedge(now, to, cactus.w[i]);
        else if(fa[to] != now && dfn[to] > dfn[now]) {
            ++ringn, top = 0;
            len[ringn] = cactus.w[i];
            bel[now] = ringn;
            int cur = to;
            while(cur != now) {
                st[++top] = cur;
                bel[cur] = ringn;
                len[ringn] += fw[cur];
                cur = fa[cur];
            }
            for(int j = 1, l = cactus.w[i], r = len[ringn]-cactus.w[i]; j <= top; ++j) {
                cur = st[j];
                d[cur][0] = l, d[cur][1] = r;
                tree.addedge(now, cur, std::min(l, r));
                l += fw[cur], r -= fw[cur];
            }
        }
}

void dfs_tree(int now, int fa) {
    for(int i = tree.head[now], to; i; i = tree.ne[i])
        if((to = tree.to[i]) != fa) {
            f[to][0] = now, fw[to] = tree.w[i];
            dis[to] = dis[now]+tree.w[i], dep[to] = dep[now]+1;
            dfs_tree(to, now);
        }
}

inline void pre_lca() {
    for(int j = 1; j < LOG; ++j)
        for(int i = 1; i <= n; ++i)
            f[i][j] = f[f[i][j-1]][j-1];
}

inline void up(int &x, int y) {
    int i = 0;
    while(y) {
        if(y&1) x = f[x][i];
        ++i, y >>= 1;
    }
}

inline int work(int u, int v) {
    int x = u, y = v;
    if(dep[u] > dep[v]) up(u, dep[u]-dep[v]);
    if(dep[v] > dep[u]) up(v, dep[v]-dep[u]);
    int add = 0;
    while(u != v) {
        for(int i = LOG-1; i >= 0; --i)
            if(f[u][i] != f[v][i])
                u = f[u][i], v = f[v][i];
        if(f[u][0] == f[v][0]) {
            if(bel[u] && bel[u] == bel[v]) {
                add -= fw[u]+fw[v];
                if(d[u][0] > d[v][0]) std::swap(u, v);
                add += std::min(len[bel[u]]-d[u][0]-d[v][1], d[u][0]+d[v][1]);
            }
            u = v = f[u][0];
        }
    }
    return dis[x]+dis[y]-dis[u]*2+add;
}

inline void solve() {
    dfs_cactus(1);
    dfs_tree(1, 0);
    pre_lca();
    while(q--) {
        int u, v;
        scanf("%d%d", &u, &v);
        printf("%d\n", work(u, v));
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    init();
    solve();
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
