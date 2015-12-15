#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <set>
#define debug(...) fprintf(stderr, __VA_ARGS__)

template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}
int OutN;
char Out[20];
template<class T>
inline void write(T x) {
    if(x < 0) putchar('-'), x = -x;
    if(x) {
        OutN = 0;
        while(x) {
            Out[OutN++] = x%10+'0';
            x /= 10;
        }
        while(OutN--)
            putchar(Out[OutN]);
    }
    else putchar('0');
}

const int N = 1e5+9;
const int INF = 2e9;

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

int col[N];
int w[N];

struct LCT {
    int fa[N], c[N][2];
    int maxl[N], maxr[N], w[N];
    int col[N], lc[N], rc[N], sum[N][2];
    std::multiset<int> m[N][2];

    inline void init() {
        for(int i = 1; i <= n; ++i)
            for(int j = 0; j < 2; ++j)
                m[i][j].insert(-INF);
        maxl[0] = maxr[0] = -INF;
    }
    
    inline void setc(int x, int p, int d) {
        c[x][d] = p, fa[p] = x;
    }

    inline void pushUp(int x) {
        lc[x] = c[x][0]?lc[c[x][0]]:col[x];
        rc[x] = c[x][1]?rc[c[x][1]]:col[x];
        sum[x][0] = sum[c[x][0]][0]+sum[c[x][1]][0]+(col[x] == 0);
        sum[x][1] = sum[c[x][0]][1]+sum[c[x][1]][1]+(col[x] == 1);
        int v = std::max(w[x], *m[x][col[x]].rbegin());
        int vl = std::max(v, c[x][0] && rc[c[x][0]] == col[x] ? maxr[c[x][0]] : -INF);
        int vr = std::max(v, c[x][1] && lc[c[x][1]] == col[x] ? maxl[c[x][1]] : -INF);
        maxl[x] = maxl[c[x][0]], maxr[x] = maxr[c[x][1]];
        if(sum[c[x][0]][col[x]^1] == 0) maxl[x] = std::max(maxl[x], vr);
        if(sum[c[x][1]][col[x]^1] == 0) maxr[x] = std::max(maxr[x], vl);
    }

    inline int d(int x) {
        return c[fa[x]][1] == x;
    }

    inline bool isRoot(int x) {
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
        while(!isRoot(x)) 
            rot(x);
        pushUp(x);
    }

    inline void access(int x) {
        for(int y = 0; x; y = x, x = fa[x]) {
            splay(x);
            if(c[x][1]) m[x][lc[c[x][1]]].insert(maxl[c[x][1]]);
            if(y) m[x][lc[y]].erase(m[x][lc[y]].find(maxl[y]));
            setc(x, y, 1), pushUp(x);
        }
    }
}lct;

inline void init() {
    read(n);
    for(int i = 1; i < n; ++i) {
        int u, v;read(u), read(v);
        G.addEdge(u, v), G.addEdge(v, u);
    }
    for(int i = 1; i <= n; ++i)
        read(lct.col[i]);
    for(int i = 1; i <= n; ++i)
        read(lct.w[i]);
}

void dfs(int now, int fa) {
    for(int i = G.head[now], to; i; i = G.ne[i])
        if((to = G.to[i]) != fa) {
            lct.fa[to] = now, dfs(to, now);
            lct.m[now][lct.lc[to]].insert(lct.maxl[to]);
        }
    lct.pushUp(now);
}

inline int query(int x) {
    lct.access(x);
    lct.splay(x);
    return lct.maxr[x];
}

inline void inverse(int x) {
    lct.access(x);
    lct.splay(x);
    lct.col[x] ^= 1;
    lct.pushUp(x);
}

inline void change(int x, int v) {
    lct.access(x);
    lct.splay(x);
    lct.w[x] = v;
    lct.pushUp(x);
}

inline void solve() {
    lct.init();
    dfs(1, 0);
    int q;read(q);
    while(q--) {
        int t, x, y;
        read(t), read(x);
        if(t == 0) write(query(x)), putchar('\n');
        else if(t == 1) inverse(x);
        else read(y), change(x, y);
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
