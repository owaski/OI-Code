#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

const int N = 3e5;
const int MOD = 998244353;

int n, c, m;
int w[N+9];
int F[N+9], G[N+9], H[N+9];
int ans;

inline int fpm(int a, int b) {
    int ret = 1;
    while(b) {
        if(b&1) ret = 1ll*ret*a%MOD;
        a = 1ll*a*a%MOD, b >>= 1;
    }
    return ret;
}

inline void DFT(int *A, int n, int f) {
    for(int i = 0; i < n; ++i) {
        int s = i, p = 0;
        for(int j = 1; j < n; j <<= 1)
            p = p<<1|(s&1), s >>= 1;
        if(p > i) std::swap(A[i], A[p]);
    }
    int u = fpm(3, (MOD-1)/n);
    w[0] = w[n] = 1;
    for(int i = 1; i < n; ++i)
        w[i] = 1ll*w[i-1]*u%MOD;
    for(int l = 2; l <= n; l <<= 1)
        for(int i = 0; i < n; i += l)
            for(int j = 0; j < (l>>1); ++j) {
                int x = A[i+j], y = 1ll*A[i+j+(l>>1)]*(f==1?w[n/l*j]:w[n-n/l*j])%MOD;
                A[i+j] = (x+y)%MOD, A[i+j+(l>>1)] = (x-y+MOD)%MOD;
            }
    if(f == -1) {
        int invn = fpm(n, MOD-2);
        for(int i = 0; i < n; ++i)
            A[i] = 1ll*A[i]*invn%MOD;
    }
}

inline void power(int *A, int n, int m) {
    DFT(A, n, 1);
    for(int i = 0; i < n; ++i)
        A[i] = fpm(A[i], m);
}

int main() {
    freopen("chocolate.in", "r", stdin);
    freopen("chocolate.out", "w", stdout);
    
    scanf("%d%d%d", &n, &c, &m);
    int rg = 1;
    while(rg <= 2*c) rg <<= 1;
    F[0] = MOD-1, F[2] = 1, power(F, rg, m);
    G[0] = G[2] = 1, power(G, rg, c-m);
    for(int i = 0; i < rg; ++i)
        H[i] = 1ll*F[i]*G[i]%MOD;
    DFT(H, rg, -1);
    int g = fpm(fpm(2, c), MOD-2);
    for(int i = 0; i < rg; ++i)
        (ans += 1ll*fpm((i-c+MOD)%MOD, n)*H[i]%MOD*g%MOD) %= MOD;
    int binom = 1;
    for(int i = 0; i < m; ++i) binom = 1ll*binom*(c-i)%MOD;
    for(int i = 1; i <= m; ++i) binom = 1ll*binom*fpm(i, MOD-2)%MOD;
    ans = 1ll*ans*binom%MOD*fpm(fpm(c, n), MOD-2)%MOD;
    printf("%d\n", ans);
    
    fclose(stdin);fclose(stdout);
    return 0;
}
