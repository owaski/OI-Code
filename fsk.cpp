#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <set>
 
#define set std::set
#define pii std::pair<int,int>
#define fi first
#define se second
#define mp make_pair
 
const int N = 100000;
 
int n, q, p, m, d, s;
 
int g, c[N+9], pos[N+9];
 
int fa1[N+9], rk1[N+9], v1[N+9], fa2[N+9], rk2[N+9], v2[N+9], f[N+9];
 
int mat[N+9];
int vis[N+9];
 
int gcd(int a, int b) {
    return b ? gcd(b, a%b) : a;
}
 
inline int find(int x, int *fa, int *val) {
    int y, anc = x;
    while(anc != fa[anc]) anc = fa[anc];
    while(x != anc) y = fa[x], fa[x] = anc, x = y;
    return val[anc];
}
 
inline int find2(int x, int *fa) {
    int y, anc = x;
    while(anc != fa[anc]) anc = fa[anc];
    while(x != anc) y = fa[x], fa[x] = anc, x = y;
    return anc;
}
 
inline void merge(int u, int v, int *fa, int *rk, int *val) {
    u = find2(u, fa), v = find2(v, fa);
    int r = val[v];
    if(rk[u] > rk[v]) std::swap(u, v);
    fa[u] = v, rk[v] = std::max(rk[v], rk[u]+1);
    val[v] = r;
}
 
inline int del(int c) {
    int v = c%g, x, y;
    int i = find(v, fa1, v1), j;
    if(i == g) y = g-v+(i = find(0, fa1, v1));else y = i-v;
    j = find((c+y)%n, fa2, v2);
    if(j == n) x = n/g-(f[(c+y)%n]-f[j = find(i, fa2, v2)]);else x = f[j]-f[(c+y)%n];
    if((j+d)%n != i) merge(j, (j+d)%n, fa2, rk2, v2);else merge(j, n, fa2, rk2, v2);
    if(find(i, fa2, v2) == n) merge(i, i+1, fa1, rk1, v1);
    return (c+1ll*d*x+y)%n;
}
 
inline void init() {
    std::cin >> n >> s >> q >> p >> m >> d, d %= n, c[0] = 0;
    for(int i = 1; i < n; ++i) c[i] = (1ll*c[i-1]*q+p)%m;
    c[0] = s, g = gcd(n, d);
    for(int i = 0; i <= g; ++i) v1[i] = fa1[i] = i, rk1[i] = 1;
    for(int i = 0; i <= n; ++i) v2[i] = fa2[i] = i, rk2[i] = 1;
    for(int i = 0; i < g; ++i)
        for(int j = 0, jn = n/g, t = i; j < jn; ++j, (t += d) %= n)
            f[t] = j;
}
 
inline void solve() {
    int ans = 0;
    for(int i = 0; i < n; ++i)
        mat[pos[i] = del(c[i])] = i, vis[i] = -1;
    for(int i = 0; i < n; ++i)
        if(vis[i] == -1) {
            if(mat[i] != i) {
                if(i == 0) ans--;
                else ans++;ans++;
                vis[i] = i;int j = mat[i];
                while(vis[j] == -1)
                    ans++, vis[j] = i, j = mat[j];
            }
        }
    std::cout << ans << std::endl;
}
 
int main() {
    int Case;
    scanf("%d", &Case);
    while(Case--)
        init(), solve();
    return 0;
}
﻿
