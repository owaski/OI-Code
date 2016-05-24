#include <bits/stdc++.h>

#define ll long long

#define SZ(x) (int)(x).size()
#define pb push_back

template<class T>inline void chkmax(T &x, const T &y) {if(x < y) x = y;}
template<class T>inline void chkmin(T &x, const T &y) {if(x > y) x = y;}

template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}
int outn;
char out[(int)2e7];
template<class T>
inline void write(T x) {
    if(x < 0) out[outn++] = '-', x = -x;
    if(x) {
        static int tmpn;
        static char tmp[20];
        tmpn = 0;
        while(x) tmp[tmpn++] = x%10+'0', x /= 10;
        while(tmpn) out[outn++] = tmp[--tmpn];
    }
    else out[outn++] = '0';
}

const int N = 1e5;

int n, qn;
struct graph {
    int sz, head[N+9], to[N*2+9], ne[N*2+9];
    inline void init(int n) {
        std::fill(head+1, head+n+1, 0), sz = 1;
    }
    inline void addedge(int u, int v) {
        to[sz] = v, ne[sz] = head[u], head[u] = sz++;
        to[sz] = u, ne[sz] = head[v], head[v] = sz++;
    }
}g;
int dep[N+9];
int d[N+9];
int p[N+9], pn;
int sz[N+9];

ll s1[N+9], s2[N+9];

#define lowbit(x) ((x) & -(x))

inline void add(int x, int v) {
    if(!x) return ;
    for(int i = x; i; i -= lowbit(i)) s1[i] += v;
    for(int i = x; i <= n; i += lowbit(i)) s2[i] += 1ll*x*v;
}

inline ll sum(int x) {
    if(!x) return 0;
    ll a = 0, b = 0;
    for(int i = x; i <= n; i += lowbit(i)) a += s1[i];
    for(int i = x-1; i; i -= lowbit(i)) b += s2[i];
    return a*x+b;
}

inline void mdf(int x, int v) {
    add(p[x]+sz[x]-1, v), add(p[x]-1, -v);
}

inline void build() {
    std::fill(s1+1, s1+n+1, 0);
    std::fill(s2+1, s2+n+1, 0);
    for(int i = 1; i <= n; ++i)
        add(i, dep[d[i]]), add(i-1, -dep[d[i]]);
}

struct node {
    node *fa, *c[2];
    int anc, val;
    node() {fa = c[0] = c[1] = 0, val = anc = 0;}
    inline void init() {
        fa = c[0] = c[1] = 0, val = anc = 0;
    }
    inline void setc(node *p, int d) {
        c[d] = p;
        if(p) p->fa = this;
    }
    inline bool dir() {
        return fa->c[1] == this;
    }
    inline bool isroot() {
        return !fa || (fa->c[0] != this && fa->c[1] != this);
    }
    inline void pushup() {
        anc = val;
        if(c[0]) anc = c[0]->anc;
    }
    inline void rot() {
        int p = dir();
        node *f = fa;
        if(f->isroot()) fa = f->fa;
        else f->fa->setc(this, f->dir());
        f->setc(c[p^1], p), setc(f, p^1);
        f->pushup();
    }
    inline void splay() {
        while(!isroot()) {
            if(!fa->isroot()) {
                if(fa->dir() == dir()) fa->rot();
                else rot();
            }
            rot();
        }
        pushup();
    }
    inline void access() {
        for(node *p = this, *q = 0; p; q = p, p = p->fa) {
            p->splay();
            if(p->c[1]) mdf(p->c[1]->anc, 1);
            if(q) mdf(q->anc, -1);
            p->setc(q, 1);
        }
    }
}*tr[N+9];

inline void init() {
    g.init(n);
    for(int i = 1; i <= n; ++i)
        tr[i]->init(), tr[i]->anc = tr[i]->val = i;
    pn = 0;
}

void dfs(int now, int fa) {
    if(fa) tr[now]->fa = tr[fa];
    d[++pn] = now, p[now] = pn, sz[now] = 1;
    for(int i = g.head[now], to; i; i = g.ne[i])
        if((to = g.to[i]) != fa)
            dep[to] = dep[now]+1, dfs(to, now), sz[now] += sz[to];
}

inline double qry(int x) {
    return 1.0*(sum(p[x]+sz[x]-1)-sum(p[x]-1))/sz[x];
}

int main() {
    int Case;
    read(Case);
    for(int i = 1; i <= N; ++i)
        tr[i] = new node;
    while(Case--) {
        read(n);
        assert(n <= N);
        init();
        for(int i = 1; i < n; ++i) {
            int u, v;
            read(u), read(v);
            g.addedge(++u, ++v);
        }
        dfs(1, 0);
        build();
        read(qn);
        while(qn--) {
            char s[3];
            scanf("%s", s);
            int u;
            read(u), u++;
            if(s[0] == 'O') tr[u]->access();
            else printf("%.6lf\n", qry(u));
        }
    }
    return 0;
}
