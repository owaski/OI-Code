#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

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

const int N = 500009;
const int K = N;
const int INF = 1e9;

int n, k;
int w[N];
int fa[N];
int sz[N];
int dep[N];
int pref[N];
int prev[N];
int pathR[N];
bool vis[N];
int dfn[N], dfnN;
int pos[N];
int tree[N<<2];
pair<int,int> chain[N][3];

struct Data {
    int w, u, v, r;
    Data() {w = u = v = r = 0;}
    Data(int _w, int _u, int _v, int _r) {w = _w, u = _u, v = _v, r = _r;} 
    inline friend bool operator < (const Data &a, const Data &b) {
        return a.w > b.w;
    }
}q[N+(K<<2)];
int qTop;

inline void init() {
    read(n), read(k), w[0] = INF;
    for(int i = 1; i <= n; ++i)
        read(w[i]);
    for(int i = 2; i <= n; ++i)
        read(fa[i]), dep[i] = dep[fa[i]]+1;
}

inline int getLca(int u, int v) {
    while(pathR[u] != pathR[v]) {
        if(dep[pathR[u]] > dep[pathR[v]]) u = fa[pathR[u]];
        else v = fa[pathR[v]];
    }
    return dep[u] > dep[v] ? v : u;
}

inline int get(int x, int l, int r, int ql, int qr) {
    if(l == ql && r == qr) return tree[x];
    int mid = (l+r)>>1;
    if(qr <= mid) return get(x<<1, l, mid, ql, qr);
    else if(ql > mid) return get(x<<1|1, mid+1, r, ql, qr);
    else {
        int p = get(x<<1, l, mid, ql, mid);
        int q = get(x<<1|1, mid+1, r, mid+1, qr);
        return w[p] < w[q] ? p : q;
    }
}

inline int getMin(int u, int d) {
    int ret = u;
    while(dep[u]-dep[pathR[u]]+1 < d) {
        if(w[ret] > w[prev[u]]) ret = prev[u];
        d -= dep[u]-dep[pathR[u]]+1, u = fa[pathR[u]];
    }
    int last = get(1, 1, n, pos[u]-d+1, pos[u]);
    if(w[ret] > w[last]) ret = last;
    return ret;
}

void makeTree(int x, int l, int r) {
    if(l == r) {
        tree[x] = dfn[l];
        return ;
    }
    int mid = (l+r)>>1;
    makeTree(x<<1, l, mid);
    makeTree(x<<1|1, mid+1, r);
    if(w[tree[x<<1]] < w[tree[x<<1|1]]) tree[x] = tree[x<<1];
    else tree[x] = tree[x<<1|1];
}

inline void pre() {
    for(int i = n; i >= 2; --i)
        sz[fa[i]] += (++sz[i]);
    for(int i = n; i >= 2; --i)
        if(sz[i] > sz[pref[fa[i]]])
            pref[fa[i]] = i;
    for(int i = 1; i <= n; ++i)
        if(!vis[i]) {
            int j = i;
            while(j) {
                vis[j] = true;
                dfn[++dfnN] = j;
                pos[j] = dfnN;
                j = pref[j];
            }
        }
    makeTree(1, 1, n);
    for(int i = 1; i <= n; ++i)
        prev[i] = w[pref[fa[i]]==i?prev[fa[i]]:0] > w[i] ? i : prev[fa[i]];
    for(int i = 1; i <= n; ++i)
        if(pref[fa[i]] == i)
            pathR[i] = pathR[fa[i]];
        else pathR[i] = i;
    for(int i = 1; i <= n; ++i) {
        int lca[3] = {0};
        for(int j = 0; j < 3; ++j)
            read(chain[i][j].first);
        for(int j = 0; j < 3; ++j)
            lca[j] = getLca(chain[i][j].first, chain[i][(j+1)%3].first);
        if(dep[lca[1]] > dep[lca[2]]) swap(lca[1], lca[2]);
        chain[i][1].second = dep[chain[i][1].first]-dep[lca[0]];
        chain[i][2].second = dep[chain[i][2].first]-dep[lca[2]];
        chain[i][0].second = dep[chain[i][0].first]-min(dep[lca[0]], dep[lca[2]])+1;
   } 
}

inline void insert(const Data &p) {
    q[qTop++] = p;
    push_heap(q, q+qTop);
}

inline void insert(int c, int u, int v) {
    if(v == 0) return ;
    int r = getMin(u, v);
    insert(Data(c+w[r], u, v, r));
}

inline void solve() {
    pre();
    for(int i = 1; i <= n; ++i)
        insert(Data(w[i], i, 1, i));
    while(k--) {
        int nr;
        Data cur = q[0];
        write(cur.w), putchar('\n');
        pop_heap(q, q+qTop), qTop--;
        for(int i = 0; i < 3; ++i)
            insert(cur.w, chain[cur.r][i].first, chain[cur.r][i].second);
        insert(cur.w-w[cur.r], cur.u, dep[cur.u]-dep[cur.r]);
        insert(cur.w-w[cur.r], fa[cur.r], cur.v-(dep[cur.u]-dep[cur.r]+1));
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("chase.in", "r", stdin);
    freopen("chase.out", "w", stdout);
#endif

    init();
    solve();
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
