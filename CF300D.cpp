#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int LOG = 33;
const int K = 1001;
const int S = 2050;
const int MOD = 7340033;
const int ROOT = 3;

int f[LOG][K];
int A[S], w[S];

inline int powerMod(int a, int b, int MOD) {
    int ret = 1, base = a;
    while(b) {
        if(b&1) ret = 1ll*ret*base%MOD;
        base = 1ll*base*base%MOD;
        b >>= 1;
    }
    return ret;
}

inline void DFT(int A[], int N, int flag) {
    for(int i = 0; i < N; ++i) {
        int p = 0, s = i;
        for(int j = 1; j < N; j <<= 1)
            p = p<<1|(s&1), s >>= 1;
        if(p > i) swap(A[i], A[p]);
    }
    w[0] = 1;
    int wn = powerMod(ROOT, (MOD-1)/N, MOD);
    for(int i = 1; i <= N; ++i) w[i] = 1ll*w[i-1]*wn%MOD;
    for(int l = 2; l <= N; l <<= 1)
        for(int i = 0; i < N; i += l)
            for(int j = 0; j < (l>>1); ++j) {
                int u = A[i+j], v = A[i+j+(l>>1)];
                if(flag == 1) v = 1ll*v*w[N/l*j]%MOD;
                else v = 1ll*v*w[N-N/l*j]%MOD;
                A[i+j] = (u+v)%MOD, A[i+j+(l>>1)] = (u-v+MOD)%MOD;
            }
    if(flag == -1) {
        int inv = powerMod(N, MOD-2, MOD);
        for(int i = 0; i < N; ++i)
            A[i] = 1ll*A[i]*inv%MOD;
    }
}

inline void sq(int A[], int N) {
    DFT(A, N, 1);
    for(int i = 0; i < N; ++i)
        A[i] = 1ll*A[i]*A[i]%MOD;
    DFT(A, N, -1);
}

inline void calc(int id) {
    int N = 2048;
    copy(f[id], f[id]+K, A), fill(A+K, A+N, 0);
    sq(A, N), fill(A+K, A+N, 0), sq(A, N);
}

inline void pre() {
    f[0][0] = 1;
    for(int i = 1; i < LOG; ++i) {
        calc(i-1), f[i][0] = 1;
        for(int j = 1; j < K; ++j)
            f[i][j] = A[j-1];
    }
}

inline int work(int n) {
    int ret = 0;
    while((n&1) && n > 1) {
        ret++, n >>= 1;
    }
    return ret;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("D.in", "r", stdin);
    freopen("D.out", "w", stdout);
#endif

    pre();
    int q;
    cin >> q;
    while(q--) {
        int n, k;
        scanf("%d%d", &n, &k);
        if(k == 0) puts("1");
        else if(!(n&1)) puts("0");
        else printf("%d\n", f[work(n)][k]);
    }
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
