#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

#define ll long long

template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}

const int N = 1000000, H = N;
const int MOD = 1e9+7;

int n, h[N+9];

int cnt[H+9];

struct graph {
    int sz, head[H+9], to[H+9], ne[H+9];
    graph() {
        sz = 1, memset(head, 0, sizeof head);
    }
    inline void addedge(int u, int v) {
        to[sz] = v, ne[sz] = head[u], head[u] = sz++;
        to[sz] = u, ne[sz] = head[v], head[v] = sz++;
    }
}g;

bool vis[H+9];

int fa[H+9];
int s[H+9], sn;

int col[H+9];// 1 for no, 2 for yes

int f[H+9][2];

inline void init() {
    read(n);
    for(int i = 1; i <= n; ++i)
        read(h[i]), cnt[h[i]]++;
}

int gcd(int a, int b) {
    return b ? gcd(b, a%b) : a;
}

void get(int now, int fa) {
    vis[now] = true;
    for(int i = g.head[now], to; i; i = g.ne[i])
        if((to = g.to[i]) != fa)
            if(vis[to]) s[++sn] = now, s[++sn] = to;
            else ::fa[to] = now, get(to, now);
}

inline int fpm(int a, int b) {
    int ret = 1;
    while(b) {
        if(b&1) ret = 1ll*ret*a%MOD;
        a = 1ll*a*a%MOD, b >>= 1;
    }
    return ret;
}

void dp(int now) {
    int ans0 = 1, ans1 = 1;
    f[now][0] = f[now][1] = 0;
    for(int i = g.head[now], to; i; i = g.ne[i])
        if(fa[to = g.to[i]] == now) {
            dp(to);
            (ans0 = 1ll*ans0*(f[to][0]+f[to][1])%MOD) %= MOD;
            (ans1 = 1ll*ans1*f[to][0]%MOD) %= MOD;
        }
    int mult = (fpm(2, cnt[now])-1+MOD)%MOD;
    if(col[now] == 1) f[now][0] = ans0;
    else if(col[now] == 2) f[now][1] = 1ll*mult*ans1%MOD;
    else {
        f[now][0] = ans0;
        f[now][1] = 1ll*mult*ans1%MOD;
    }
}

int bf(int now, int root) {
    if(now == sn+1) {
        dp(root);
        return (f[root][0]+f[root][1])%MOD;
    }
    int ret = 0, x = s[now];
    if(!col[x]) {
        bool flag = true;
        col[x] = 1, ret = bf(now+1, root);
        col[x] = 2;std::vector<int> mdf;
        for(int i = g.head[x]; i; i = g.ne[i]) {
            int to = g.to[i];
            if(!col[to]) mdf.push_back(to);
            else if(col[to] != 1) {
                flag = false;
                break;
            }
        }
        if(flag) {
            for(int i = 0; i < mdf.size(); ++i)
                col[mdf[i]] = 1;
            (ret += bf(now+1, root)) %= MOD;
            for(int i = 0; i < mdf.size(); ++i)
                col[mdf[i]] = 0;
        }
        col[x] = 0;
    }
    else ret = bf(now+1, root);
    return ret;
}

inline void solve() {
    for(int m = 1; m <= H; ++m)
        for(int n = ceil(sqrt(std::max(1ll, 1ll*m*m-H))); n < m && 2ll*n*m <= H; ++n)
            if(((n+m)&1) && gcd(n, m) == 1 && cnt[m*m-n*n] && cnt[2*n*m])
                g.addedge(m*m-n*n, 2*n*m);
    int ans = 1;
    for(int i = 1; i <= H; ++i)
        if(!vis[i] && cnt[i]) {
            sn = 0, get(i, 0);
            std::sort(s+1, s+sn+1);
            sn = std::unique(s+1, s+sn+1)-s-1;
            ans = 1ll*ans*bf(1, i)%MOD;
        }
    ans = (ans-1+MOD)%MOD;
    std::cout << ans << std::endl;
}

int main() {
    init();
    solve();
    return 0;
}
