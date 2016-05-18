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
    while((c=getchar()) >= '0' && c <= '9') x= x*10+c-'0';
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

const int N = 16;
const int MOD = 998244353;

int n, m;
int adj[N];
int g[1<<N][N];
int cir[1<<N];
int cnt[1<<N];
int way[N];
int power;
int ans;

void dfs(int s, int x, int v) {
    if(x == n) {
        (cnt[s] += MOD - v) %= MOD;
        (ans += 1ll * power * v % MOD) %= MOD;
        return ;
    }
    dfs(s, x+1, v);
    if(!(s & (1 << x)))
        dfs(s | (1 << x), x + 1, 1ll * (MOD-v) * way[x] % MOD);
}

int main() {
    read(n), read(m);
    for(int i = 0; i < m; ++i) {
        int u, v;
        read(u), read(v), u--, v--;
        adj[u] |= 1 << v;
        adj[v] |= 1 << u;
    }
    for(int i = 0; i < n; ++i)
        g[1 << i][i] = 1;
    for(int s = 1; s < (1 << n); ++s) {
        int p = __builtin_ctz(s);
        bool flag = __builtin_popcount(s) >= 3;
        for(int i = 0; i < n; ++i)
            if(g[s][i]) {
                int v = g[s][i];
                for(int j = p; j < n; ++j)
                    if((adj[i] & (1 << j)) && !(s & (1 << j)))
                        (g[s | (1 << j)][j] += v) %= MOD;
                if(flag && (adj[p] & (1 << i))) (cir[s] += v) %= MOD;
            }
    }
    for(int s = 1; s < (1 << n); ++s)
        cnt[s] = cir[s] = 1ll * cir[s] * ((MOD + 1) >> 1) % MOD;
    for(int s = 1; s < (1 << n); ++s)
        if(cnt[s]) {
            for(int i = 0; i < n; ++i)
                if(!(s & (1 << i)))
                    way[i] = __builtin_popcount(adj[i] & s);
                else way[i] = 0;
            power = 1 << __builtin_popcount(s);
            dfs(s, 0, cnt[s]);
        }
    std::cout << ans << std::endl;
    return 0;
}
