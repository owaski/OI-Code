#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}

int OutN;
char Out[20];

template<class T>
inline void write(T x) {
    if(x < 0) putchar('-'), x = -x;;
    if(x) {
        OutN = 0;
        while(x) {
            Out[OutN++] = x%10+'0';
            x /= 10;
        }
        while(OutN--)
            putchar(Out[OutN]);
    }
    else putchar('0');
}

const int N = 300009;
const int MOD = 998244353;
const int ROOT = 3;

int n, m;
int c[N];
int r[N];
int ans[N];
int w[N];

inline void init() {
    read(n), read(m);
    for(int i = 1; i <= n; ++i) {
        int x;read(x);c[x] = 1;
    }
}

inline int powerMod(int a, int b, int MOD) {
    int ret = 1, base = a;
    while(b) {
        if(b&1) ret = 1ll*ret*base%MOD;
        base = 1ll*base*base%MOD;
        b >>= 1;
    }
    return ret;
}

inline void DFT(int a[], int N, int flag) {
    for(int i = 0; i < N; ++i) {
        int p = 0, s = i;
        for(int j = 1; j < N; j <<= 1)
            p = p<<1|(s&1), s >>= 1;
        if(p > i) swap(a[i], a[p]);
    }
    w[0] = 1;
    int wn = powerMod(ROOT, (MOD-1)/N, MOD);
    for(int i = 1; i <= N; ++i)
        w[i] = 1ll*w[i-1]*wn%MOD;
    for(int l = 2; l <= N; l <<= 1)
        for(int i = 0; i < N; i += l)
            for(int j = 0; j < (l>>1); ++j) {
                int u = a[i+j], v = a[i+j+(l>>1)];
                if(flag == 1) v =1ll*v*w[N/l*j]%MOD;
                else v = 1ll*v*w[N-N/l*j]%MOD;
                a[i+j] = (u+v)%MOD, a[i+j+(l>>1)] = (u-v+MOD)%MOD;
            }
    if(flag == -1) {
        int inv = powerMod(N, MOD-2, MOD);
        for(int i = 0; i < N; ++i)
            a[i] = 1ll*a[i]*inv%MOD;
    }
}

int tmp[N];

void getInv(int a[], int n, int b[]) {
    if(n == 1) {
        b[0] = powerMod(a[0], MOD-2, MOD);
        return ;
    }
    getInv(a, (n+1)>>1, b);
    int N = 1;
    while(N < 2*n) N <<= 1;
    copy(a, a+n, tmp), fill(tmp+n, tmp+N, 0);
    DFT(tmp, N, 1), DFT(b, N, 1);
    for(int i = 0; i < N; ++i)
        b[i] = 1ll*b[i]*(2-1ll*b[i]*tmp[i]%MOD+MOD)%MOD;
    DFT(b, N, -1), fill(b+n, b+N, 0);
}

int tmp2[N];

void getRoot(int a[], int n, int b[]) {
    if(n == 1) {
        b[0] = 1;
        return ;
    }
    getRoot(a, (n+1)>>1, b);
    int N = 1;
    while(N < 2*n) N <<= 1;
    fill(tmp2, tmp2+N, 0), getInv(b, n, tmp2);
    copy(a, a+n, tmp), fill(tmp+n, tmp+N, 0);
    DFT(tmp, N, 1),  DFT(tmp2, N, 1);
    int inv2 = powerMod(2, MOD-2, MOD);
    for(int i = 0; i < N; ++i)
        tmp[i] = 1ll*tmp[i]*tmp2[i]%MOD;
    DFT(tmp, N, -1), fill(tmp+n, tmp+N, 0);
    for(int i = 0; i < n; ++i)
        b[i] = 1ll*inv2*(b[i]+tmp[i])%MOD;
}

inline void solve() {
    c[0] = 1;
    for(int i = 1; i <= m; ++i)
        c[i] = (-4*c[i]+MOD)%MOD;
    getRoot(c, m+1, r);
    r[0]++;
    int inv2 = powerMod(2, MOD-2, MOD);
    for(int i = 0; i <= m; ++i)
        r[i] = 1ll*inv2*r[i]%MOD;
    getInv(r, m+1, ans);
    for(int i = 1; i <= m; ++i)
        write(ans[i]), putchar('\n');
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("E.in", "r", stdin);
    freopen("E.out", "w", stdout);
#endif

    init();
    solve();
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
