#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

#define ll long long

const int N = 2000;

int n, m;

struct graph {
    int sz, head[N+9], to[N*2+9], ne[N*2+9];
    ll w[N*2+9];
    graph() {
        sz = 1, memset(head, 0, sizeof head);
    }
    inline void addedge(int u, int v, int _w) {
        to[sz] = v, w[sz] = _w, ne[sz] = head[u], head[u] = sz++;
        to[sz] = u, w[sz] = _w, ne[sz] = head[v], head[v] = sz++;
    }
}g;

int size[N+9];
ll f[N+9][N+9];

template<class T>
inline void chkmax(T &x, T y) {
    if(x < y) x = y;
}

void dfs(int now, int fa) {
    size[now] = 1;
    for(int i = g.head[now], to; i; i = g.ne[i])
        if((to = g.to[i]) != fa) {
            dfs(to, now);
            int x = std::min(m, size[now]);
            int y = std::min(m, size[to]);
            for(int j = x; j >= 0; --j)
                for(int k = y; k >= 0; --k)
                    chkmax(f[now][j+k], f[now][j]+f[to][k]+g.w[i]*(k*(m-k)+(size[to]-k)*(n-m-(size[to]-k))));
            size[now] += size[to];
        }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%d%d", &n, &m);
    for(int i = 1; i < n; ++i) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        g.addedge(u, v, w);
    }
    dfs(1, 0);
    std::cout << f[1][m] << std::endl;
    
    fclose(stdin);fclose(stdout);
    return 0;
}
