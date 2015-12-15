#include <set>
#include <ctime>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define SZ(x) (int)(x).size()
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

const int N = 100009;
const int LOG = 20;
const int INF = 0x3f3f3f3f;

int n;
struct Graph {
    int sz;
    int head[N];
    int to[N<<1];
    int ne[N<<1];
    int fr[N<<1];
    Graph() {
        sz = 1;
        memset(head, 0, sizeof head);
    }
    inline void addEdge(int u, int v) {
        to[sz] = v, fr[head[u]] = sz, ne[sz] = head[u], head[u] = sz++;
    }
#define inv(x) ((x-1)^1)+1
    inline void delEdge(int e) {
        int ie = inv(e), u = to[e], v = to[ie];
        if(e == head[v]) head[v] = ne[e];
        if(ie == head[u]) head[u] = ne[ie];
        ne[fr[e]] = ne[e], fr[ne[e]] = fr[e];
        ne[fr[ie]] = ne[ie], fr[ne[ie]] = fr[ie];
    }
}G;
int q[N];
bool vis[N];
std::multiset<int> h[N];
std::vector<int> fa[N];
std::vector<int> dis[N];
int f[N], g[N], sz[N];
bool col[N];

inline void init() {
    read(n);
    for(int i = 1; i < n; ++i) {
        int u, v;read(u), read(v);
        G.addEdge(u, v), G.addEdge(v, u);
    }
}

inline int getRoot(int root) {
    int h = 0, t = 0;
    q[++t] = root, vis[root] = true;
    while(h < t) {
        int now = q[++h];sz[now] = 0;
        for(int i = G.head[now]; i; i = G.ne[i])
            if(!vis[G.to[i]]) {
                f[G.to[i]] = now;
                vis[G.to[i]] = true;
                q[++t] = G.to[i];
            }
    }
    for(int i = 1; i <= t; ++i)
        vis[q[i]] = false;
    for(int i = t; i >= 1; --i) {
        int now = q[i];g[now] = 0;
        sz[f[now]] += (++sz[now]);
        for(int j = G.head[now]; j; j = G.ne[j])
            if(G.to[j] != f[now])
                g[now] = std::max(g[now], sz[G.to[j]]);
    }
    int ret = q[1];
    for(int i = 1; i <= t; ++i) {
        int now = q[i];
        g[now] = std::max(g[now], sz[root]-sz[now]);
        if(g[ret] >= g[now]) ret = now;
    }
    for(int i = 1; i <= t; ++i)
        fa[q[i]].push_back(ret);
    ::h[ret].insert(INF);
    h = t = 0, q[++t] = ret, vis[ret] = true, dis[ret].push_back(0);
    while(h < t) {
        int now = q[++h];
        for(int i = G.head[now]; i; i = G.ne[i])
            if(!vis[G.to[i]]) {
                vis[G.to[i]] = true;
                dis[G.to[i]].push_back(dis[now][SZ(dis[now])-1]+1);
                q[++t] = G.to[i];
            }
    }
    for(int i = 1; i <= t; ++i)
        vis[q[i]] = false;
    return ret;
}

void dfs(int now, int fe) {
    if(fe) G.delEdge(fe);
    now = getRoot(now);
    for(int i = G.head[now]; i; i = G.ne[i])
        dfs(G.to[i], i);
}

int sum;

inline void adjust(int x, int i, int f) {
    if(f) h[fa[x][i]].insert(dis[x][i]);
    else h[fa[x][i]].erase(h[fa[x][i]].lower_bound(dis[x][i]));
}

inline void modify(int x) {
    col[x] ^= true;
    if(col[x]) sum++;
    else sum--;
    for(int i = 0; i < SZ(fa[x]); ++i)
        if(col[x]) adjust(x, i, 1);
        else adjust(x, i, 0);
}

inline int query(int x) {
    if(sum == 0) return -1;
    int ret = INF;
    for(int i = 0; i < SZ(fa[x]); ++i)
        ret = std::min(ret, dis[x][i]+*h[fa[x][i]].begin());
    return ret;
}

inline void solve() {
    dfs(1, 0);
    int q;read(q);
    while(q--) {
        int t, x;
        read(t), read(x);
        if(t == 0) modify(x);
        else write(query(x)), putchar('\n');
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    init(); 
    solve();
    debug("%lfs\n", (double)clock()/CLOCKS_PER_SEC);
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
