#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
 
const int N = 100, M = 10000;
const double eps = 1e-10;
 
int n, m;
int deg[N+9];
struct graph {
    int sz, head[N+9], to[M*2+9], w[M*2+9], ne[M*2+9];
    graph() {
        sz = 1, memset(head, 0, sizeof head);
    }
    inline void addedge(int u, int v, int _w) {
        to[sz] = v, ne[sz] = head[u], w[sz] = _w, head[u] = sz++;
        if(u != v) to[sz] = u, ne[sz] = head[v], w[sz] = _w, head[v] = sz++;
    }
}g;
double h[N+9][N+9];
 
inline bool zero(double x) {
    return -eps <= x && x <= eps;
}
 
inline void init() {
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= m; ++i) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        g.addedge(u, v, w);
        if(u != v) deg[u]++, deg[v]++;
        else deg[u]++;
    }
}
 
inline double calc(int x) {
    memset(h, 0, sizeof h);
    h[n][n] = 1;
    for(int i = 1; i < n; ++i) {
        double t = 1.0/deg[i];
        h[i][i] = -1;
        for(int j = g.head[i]; j; j = g.ne[j])
            if(g.w[j]&(1<<x)) h[i][g.to[j]] -= t, h[i][n+1] -= t;
            else h[i][g.to[j]] += t;
    }
    for(int i = 1; i <= n; ++i) {
        int j = i;
        while(j <= n && zero(h[j][i]))
            j++;
        if(j == n+1) continue;
        if(i != j)
            for(int k = i; k <= n+1; ++k)
                std::swap(h[i][k], h[j][k]);
        for(int k = 1; k <= n; ++k)
            if(k != i && !zero(h[k][i])) {
                double t = h[k][i]/h[i][i];
                for(int p = i; p <= n+1; ++p)
                    h[k][p] -= h[i][p]*t;
            }
    }
    return h[1][n+1]/h[1][1];
}
 
inline void solve() {
    double ans = 0;
    for(int i = 0; i <= 31; ++i)
        ans += calc(i)*(1<<i);
    printf("%.3lf\n", ans);
}
 
int main() {
    init();
    solve();
    return 0;
}
