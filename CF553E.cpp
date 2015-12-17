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

const int N = 59;
const int M = 109;
const int T = 70000;
const int INF = 1e9;
const double pi = acos(-1.0);
int n, m, t, x;

struct Graph {
    int sz, head[N], to[M], c[M], ne[M];
    double p[M][T], s[M][T];
    Graph() {
        sz = 1;
        memset(head, 0, sizeof head);
    }
    inline int addEdge(int u, int v, int d) {
        to[sz] = v, c[sz] = d, ne[sz] = head[u], head[u] = sz;
        return sz++;
    }
}G;

int dis[N][N];
double g[N];
double f[N][T];
double h[M][T];

struct cpx {
    double a, b;
    cpx() {a = b = 0;}
    cpx(double _a, double _b) {a = _a, b = _b;}
    inline friend cpx operator + (const cpx &p, const cpx &q) {
        return cpx(p.a+q.a, p.b+q.b);
    }
    inline friend cpx operator - (const cpx &p, const cpx &q) {
        return cpx(p.a-q.a, p.b-q.b);
    }
    inline friend cpx operator * (const cpx &p, const cpx &q) {
        return cpx(p.a*q.a-p.b*q.b, p.a*q.b+p.b*q.a);
    }
    inline friend cpx operator / (const cpx &p, const double &k) {
        return cpx(p.a/k, p.b/k);
    }
}A[T], B[T];

inline void init() {
    read(n), read(m), read(t), read(x);
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j < i; ++j)
            dis[i][j] = dis[j][i] = INF;
    for(int i = 1; i <= n; ++i)
        std::fill(f[i], f[i]+t+1, INF);
    for(int i = 1; i <= m; ++i) {
        int a, b, c;
        read(a), read(b), read(c);
        int id = G.addEdge(a, b, c);
        for(int j = 1, x; j <= t; ++j) {
            read(x), G.p[id][j] = x/100000.0;
        }
        for(int j = t; j >= 1; --j)
            G.s[id][j] = G.s[id][j+1]+G.p[id][j];
        dis[a][b] = std::min(dis[a][b], c);
    }
}

inline void floyd() {
    for(int k = 1; k <= n; ++k)
        for(int i = 1; i <= n; ++i)
            for(int j = 1; j <= n; ++j)
                dis[i][j] = std::min(dis[i][j], dis[i][k]+dis[k][j]);
    for(int i = 1; i <= n; ++i)
        g[i] = dis[i][n]+x;
}

inline void DFT(cpx A[], int N, int flag) {
    for(int i = 0; i < N; ++i) {
        int p = 0, s = i;
        for(int j = 1; j < N; j <<= 1)
            p = p<<1|(s&1), s >>= 1;
        if(p > i) std::swap(A[i], A[p]);
    }
    for(int l = 2; l <= N; l <<= 1) {
        cpx wn = cpx(cos(2*pi*flag/l), sin(2*pi*flag/l));
        for(int i = 0; i < N; i += l) {
            cpx w = cpx(1, 0);
            for(int j = 0; j < (l>>1); ++j) {
                cpx u = A[i+j], v = A[i+j+(l>>1)]*w;
                A[i+j] = u+v, A[i+j+(l>>1)] = u-v;
                w = w*wn;
            }
        }
    }
    if(flag == -1)
        for(int i = 0; i < N; ++i)
            A[i] = A[i]/N;
}

inline void update(int e, int l, int r) {
    int v = G.to[e], mid = (l+r)>>1, rg = r-l, N = 1;
    while(N <= rg+mid-l) N <<= 1;
    for(int i = l; i <= mid; ++i) A[i-l].a = f[v][i], A[i-l].b = 0;
    for(int i = mid-l+1; i < N; ++i) A[i].a = A[i].b = 0;
    B[0].a = B[0].b = 0;
    for(int i = 1; i <= rg; ++i) B[i].a = G.p[e][i], B[i].b = 0;
    for(int i = rg+1; i < N; ++i) B[i].a = B[i].b = 0;
    DFT(A, N, 1), DFT(B, N, 1);
    for(int i = 0; i < N; ++i) A[i] = A[i]*B[i];
    DFT(A, N, -1);
    for(int i = mid+1; i <= r; ++i)
        h[e][i] += A[i-l].a;
}

void solve(int l, int r) {
    if(l == r) {
        for(int i = 1; i <= n; ++i) {
            double val = i==n?0:INF;
            for(int j = G.head[i]; j; j = G.ne[j])
                val = std::min(val, G.c[j]+h[j][l]+g[G.to[j]]*G.s[j][l+1]);
            f[i][l] = val;
        }
        return ;
    }
    int mid = (l+r)>>1;
    solve(l, mid);
    for(int i = 1; i < G.sz; ++i)
        update(i, l, r);
    solve(mid+1, r);
}

inline void solve() {
    floyd();
    solve(0, t);
    printf("%.10lf\n", f[1][t]);
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
