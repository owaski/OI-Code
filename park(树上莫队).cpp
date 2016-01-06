#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>

#define debug(...) fprintf(stderr, __VA_ARGS__)

#define int64 long long

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
        int tmpn = 0, tmp[20];
        while(x) {
            tmp[tmpn++] = x%10+'0';
            x /= 10;
        }
        while(tmpn)
            out[outn++] = tmp[--tmpn];
    }
    else out[outn++] = '0';
}

const int N = 100009;
const int LOG = 20;
const int M = 100009;
const int Q = 100009;

int n, m, qn;
int v[M], w[N], c[N];

int col[N];

int size;

struct Graph {
    int sz, head[N], to[N<<1], ne[N<<1];
    Graph() {
        sz = 1, memset(head, 0, sizeof head);
    }
    inline void add_edge(int u, int v) {
        to[sz] = v, ne[sz] = head[u], head[u] = sz++;
        to[sz] = u, ne[sz] = head[v], head[v] = sz++;
    }
}G;

int dfn[N<<1];
int curtime;
int pos[N][2];
int st[N], top;
int dep[N];
int fa[N];
int f[N][LOG];
int bel[N], tot;

int cross;
bool inner[N];

struct Option {
    int x, y, t;
    Option() {x = y = t = 0;}
    Option(int _x, int _y, int _t):x(_x), y(_y), t(_t) {}
}mdf[Q], qry[Q];
int mdfn, qryn;

int cnt[M];
int64 curans;

int64 ans[Q];

int curu, curv, curlca;

inline void init() {
    read(n), read(m), read(qn);
    for(int i = 1; i <= m; ++i)
        read(v[i]);
    for(int i = 1; i <= n; ++i)
        read(w[i]);
    for(int i = 1; i < n; ++i) {
        int u, v;
        read(u), read(v);
        G.add_edge(u, v);
    }
    for(int i = 1; i <= n; ++i)
        read(c[i]), col[i] = c[i];
    for(int i = 1; i <= qn; ++i) {
        int t, x, y;
        read(t), read(x), read(y);
        if(t == 0) mdf[++mdfn] = Option(x, y, i);
        else qry[++qryn] = Option(x, y, i);
        ans[i] = -1;
    }
    size = pow(n, 2.0/3)*0.6;
    //debug("%d\n", size);
}

int dfs(int now) {
    int ret = 0; 
    st[++top] = now;
    dfn[++curtime] = now, pos[now][0] = curtime;
    for(int i = G.head[now], to; i; i = G.ne[i])
        if((to = G.to[i]) != fa[now]) {
            f[to][0] = fa[to] = now;
            dep[to] = dep[now]+1;
            ret += dfs(to);
            if(ret >= size) {
                ret = 0, ++tot;
                while(st[top] != now)
                    bel[st[top--]] = tot;
            }
        }
    dfn[++curtime] = now, pos[now][1] = curtime;
    return ret+1;
}

inline bool cmp(const Option &p, const Option &q) {
    return bel[p.x] < bel[q.x] || (bel[p.x] == bel[q.x] && bel[p.y] < bel[q.y]) || (bel[p.x] == bel[q.x] && bel[p.y] == bel[q.y] && p.t < q.t);
}

inline void prelca() {
    for(int j = 1; j < 20; ++j)
        for(int i = 1; i <= n; ++i)
            f[i][j] = f[f[i][j-1]][j-1];
    /*
    for(int i = 1; i <= n; ++i)
        for(int j = 0; j < 20; ++j)
            debug("fa[%d][%d] = %d\n", i, j, f[i][j]);
    */
}

inline void decomposition() {
    dfs(1);
    while(top)
        bel[st[top--]] = tot;
    /*
    for(int i = 1; i <= n; ++i)
        debug("bel[%d] = %d\n", i, bel[i]);
    */
    for(int i = 1; i <= qryn; ++i)
        if(bel[qry[i].x] > bel[qry[i].y])
            std::swap(qry[i].x, qry[i].y);
    std::sort(qry+1, qry+qryn+1, cmp);
    prelca();
}

inline bool is_anc(int a, int b) {
    return pos[a][0] <= pos[b][0] && pos[b][1] <= pos[a][1];
}

inline bool on_road(int a) {
    return is_anc(curlca, a) && (is_anc(a, curu) || is_anc(a, curv));
}

inline void modify(int x, int val) {
    int a = col[x], b = col[x]=val;
    if(on_road(x)) {
        curans -= 1ll*w[cnt[a]--]*v[a];
        curans += 1ll*w[++cnt[b]]*v[b];
    }
}

inline void inv(int x) {
    if(inner[x]) {
        inner[x] = false;
        curans -= 1ll*w[cnt[col[x]]--]*v[col[x]];
    }
    else {
        inner[x] = true;
        curans += 1ll*w[++cnt[col[x]]]*v[col[x]];
    }
}

inline void move_up(int &x) {
    if(!cross) {
        if(inner[x] && !inner[fa[x]]) cross = x;
        else if(!inner[x] && inner[fa[x]]) cross = fa[x];
    }
    inv(x), x = fa[x];
}

inline void move(int a, int b) {
    if(a == b) return ;
    cross = 0;
    if(inner[b]) cross = b;
    while(dep[a] > dep[b]) move_up(a);
    while(dep[b] > dep[a]) move_up(b);
    while(a != b) move_up(a), move_up(b);
    inv(a), inv(cross);
}

inline void up(int &x, int y) {
    int i = 0;
    while(y) {
        if(y&1) x = f[x][i];
        i++, y >>= 1;
    }
}

inline int get_lca(int u, int v) {
    if(dep[u] < dep[v]) std::swap(u, v);
    if(dep[u] > dep[v]) up(u, dep[u]-dep[v]);
    while(u != v) {
        for(int i = 19; i >= 0; --i)
            if(f[u][i] != f[v][i])
                u = f[u][i], v = f[v][i];
        if(fa[u] == fa[v])
            u = v = fa[u];
    }
    return u;
}

inline void solve() {
    decomposition();
    curu = curv = curlca = 1, inner[1] = true;
    cnt[c[1]]++, curans = 1ll*w[1]*v[c[1]];
    for(int i = 1; i <= qryn;) {
        int j = i, k = 0;
        while(j < qryn && bel[qry[i].x] == bel[qry[j+1].x] && bel[qry[i].y] == bel[qry[j+1].y]) j++;
        while(i <= j) {
            while(k < mdfn && mdf[k+1].t < qry[i].t) ++k, modify(mdf[k].x, mdf[k].y);
            move(curu, qry[i].x), curu = qry[i].x, move(curv, qry[i].y), curv = qry[i].y;
            curlca = get_lca(curu, curv), ans[qry[i].t] = curans, ++i;
        }
        while(k)
            modify(mdf[k].x, c[mdf[k].x]), k--;
    }
    for(int i = 1; i <= qn; ++i)
        if(ans[i] != -1)
            write(ans[i]), out[outn++] = '\n';
    puts(out);
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("park.in", "r", stdin);
    freopen("park.out", "w", stdout);
#endif

    init();
    solve();
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
