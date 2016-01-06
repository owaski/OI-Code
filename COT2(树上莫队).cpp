#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>

#define debug(...) fprintf(stderr, __VA_ARGS__)

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

const int N = 40009;
const int M = 100009;
const int S = 130;
const int T = N/S+9;

int n, m;
int col[N];
int val[N], valn;

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

struct Query {
    int u, v, id;
    Query() {u = v = id = 0;}
    Query(int _u, int _v, int _id):u(_u), v(_v), id(_id) {}
    inline void input() {
        read(u), read(v);
    }
}q[M];

int fa[N];
int dep[N];
int timef[N];
int curtime;
int st[N], top;
int bel[N], tot;

int cross;
bool inner[N];
int cnt[N];
int curans;
int ans[M];

inline void init() {
    read(n), read(m);
    for(int i = 1; i <= n; ++i)
        read(col[i]), val[++valn] = col[i];
    for(int i = 1; i < n; ++i) {
        int u, v;
        read(u), read(v);
        G.add_edge(u, v);
    }
    for(int i = 1; i <= m; ++i)
        q[i].input(), q[i].id = i;
    size = n/sqrt(m);
}

inline void discrete() {
    std::sort(val+1, val+valn+1);
    valn = std::unique(val+1, val+valn+1)-val-1;
    for(int i = 1; i <= n; ++i)
        col[i] = std::lower_bound(val+1, val+valn+1, col[i])-val;
}

int dfs(int now) {
    int ret = 0;
    st[++top] = now, timef[now] = ++curtime;
    for(int i = G.head[now], to; i; i = G.ne[i])
        if((to = G.to[i]) != fa[now]) {
            fa[to] = now;
            dep[to] = dep[now]+1;
            ret += dfs(to);
            if(ret >= size) {
                ++tot, ret = 0;
                while(st[top] != now)
                    bel[st[top--]] = tot;
            }
        }
    return ret+1;
}

inline bool cmp(const Query &p, const Query &q) {
    return bel[p.u] < bel[q.u] || (bel[p.u] == bel[q.u] && timef[p.v] < timef[q.v]);
}

inline void decomposition() {
    dfs(1);
    while(top)
        bel[st[top--]] = tot;
    for(int i = 1; i <= m; ++i)
        if(bel[q[i].u] > bel[q[i].v])
            std::swap(q[i].u, q[i].v);
    std::sort(q+1, q+m+1, cmp);
}

inline void inv(int x) {
    if(inner[x]) {
        inner[x] = false;
        curans -= (--cnt[col[x]]) == 0;
    }
    else {
        inner[x] = true;
        curans += (cnt[col[x]]++) == 0;
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
    while(dep[a] < dep[b]) move_up(b);
    while(a != b) move_up(a), move_up(b);
    inv(a), inv(cross);
}

inline void solve() {
    discrete();
    decomposition();
    int u = 1, v = 1;
    cnt[col[1]]++, curans = 1, inner[1] = true;
    for(int i = 1; i <= m; ++i) {
        move(u, q[i].u), u = q[i].u;
        move(v, q[i].v), v = q[i].v;
        ans[q[i].id] = curans;
    }
    for(int i = 1; i <= m; ++i)
        write(ans[i]), out[outn++] = '\n';
    puts(out);
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
