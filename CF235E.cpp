#include <bits/stdc++.h>

#define debug(...) fprintf(stderr, __VA_ARGS__)

#define int64 long long

const int N = 2009;
const int MOD = 1073741824;

int n;
int a, b, c;
int gcd[N][N];
int64 ans;

bool is[N];
int prime[N], tot;
int mu[N];

inline void init() {
    std::cin >> a >> b >> c;
}

inline int Gcd(int a, int b) {
    return b ? Gcd(b, a%b) : a;
}

inline void prepare() {
    n = std::max(a, std::max(b, c));
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= n; ++j)
            gcd[i][j] = Gcd(i, j);
    mu[1] = 1;
    for(int i = 2; i <= n; ++i) {
        if(!is[i]) {
            prime[++tot] = i;
            mu[i] = -1;
        }
        for(int j = 1; j <= tot && prime[j]*i <= n; ++j) {
            int nex = prime[j]*i;
            is[nex] = true;
            if(i%prime[j] == 0) break;
            mu[nex] = -mu[i];
        }
    }
}

inline int calc(int n, int x) {
    int ret = 0;
    for(int i = 1; i <= n; ++i)
        if(gcd[x][i] == 1)
            ret += n/i;
    return ret;
}

inline void solve() {
    prepare();
    for(int i = 1; i <= a; ++i)
        for(int d = std::min(b, c); d >= 1; --d)
            if(gcd[i][d] == 1 && mu[d])
                ans += 1ll*mu[d]*(a/i)*calc(b/d, i)*calc(c/d, i);
    std::cout << (ans&(MOD-1)) << std::endl;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    init();
    solve();
    debug("%lfs\n", (double)clock()/CLOCKS_PER_SEC);
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
