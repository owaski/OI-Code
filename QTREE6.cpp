#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <queue>
#define debug(...) fprintf(stderr, __VA_ARGS__)

const int N = 1e5+9;

int n;
struct Graph {
    int sz;
    int head[N];
    int to[N<<1];
    int ne[N<<1];
    Graph() {
        sz = 1;
        memset(head, 0, sizeof head);
    }
    inline void addEdge(int u, int v) {
        to[sz] = v, ne[sz] = head[u], head[u] = sz++;
    }
}G;

struct LCT {
    int fa[N], c[N][2];
    int maxl[N], maxr[N], sum[N][2], col[N];
    int s[N][2], lc[N], rc[N];

    inline void pushUp(int x) {
        lc[x] = c[x][0]?lc[c[x][0]]:col[x];
        rc[x] = c[x][1]?rc[c[x][1]]:col[x];
        sum[x][0] = (col[x] == 0)+sum[c[x][0]][0]+sum[c[x][1]][0];
        sum[x][1] = (col[x] == 1)+sum[c[x][0]][1]+sum[c[x][1]][1];
        int ml = 1+s[x][col[x]]+(rc[c[x][0]] == col[x] ? maxr[c[x][0]] : 0);
        int mr = 1+s[x][col[x]]+(lc[c[x][1]] == col[x] ? maxl[c[x][1]] : 0);
        maxl[x] = maxl[c[x][0]]+(sum[c[x][0]][col[x]^1] == 0 ? mr : 0);
        maxr[x] = maxr[c[x][1]]+(sum[c[x][1]][col[x]^1] == 0 ? ml : 0);
    }
    
    inline void setc(int x, int p, int d) {
        c[x][d] = p, fa[p] = x;
    }

    inline int d(int x) {
        return c[fa[x]][1] == x;
    }

    inline int isRoot(int x) {
        return fa[x] == 0 || (c[fa[x]][0] != x && c[fa[x]][1] != x);
    }

    inline void rot(int x) {
        int f = fa[x], ff = fa[f];
        int p = d(x), pp = d(f);
        if(isRoot(f)) fa[x] = ff;
        else setc(ff, x, pp);
        setc(f, c[x][p^1], p), setc(x, f, p^1);
        pushUp(f);
    }

    inline void splay(int x) {
        while(!isRoot(x)) {
            if(!isRoot(fa[x])) {
                if(d(x) == d(fa[x])) rot(fa[x]);
                else rot(x);
            }
            rot(x);
        }
        pushUp(x);
    }

    inline void access(int x) {
        for(int y = 0; x; y = x, x = fa[x]) {
            splay(x);
            s[x][lc[c[x][1]]] += maxl[c[x][1]];
            s[x][lc[y]] -= maxl[y];
            setc(x, y, 1), pushUp(x);
        }
    }
}lct;

inline void init() {
    scanf("%d", &n);
    for(int i = 1; i < n; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        G.addEdge(u, v), G.addEdge(v, u);
    }
}

void dfs(int now, int fa) {
    for(int i = G.head[now]; i; i = G.ne[i])
        if(G.to[i] != fa) {
            lct.fa[G.to[i]] = now, dfs(G.to[i], now);
            lct.s[now][lct.lc[G.to[i]]] += lct.maxl[G.to[i]];
        }
    lct.pushUp(now);
}

inline void modify(int x) {
    lct.access(x);
    lct.splay(x);
    lct.col[x] ^= 1;
    lct.pushUp(x);
}

inline int query(int x) {
    lct.access(x);
    lct.splay(x);
    return lct.maxr[x];
}

inline void solve() {
    dfs(1, 0);
    int q;scanf("%d", &q);
    while(q--) {
        int t, x;
        scanf("%d%d", &t, &x);
        if(t) modify(x);
        else printf("%d\n", query(x));
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
