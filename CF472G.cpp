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
    if(x < 0) putchar('-'), x = -x;
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
const double pi = acos(-1.0);

char s[N];
int lA, lB;
int A[N], B[N];
int sum[N][110];
int bS, bN;
struct cpx {
    double a, b;
    cpx(double _a = 0, double _b = 0) {a = _a, b = _b;}
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
}T1[N], T2[N];

inline void input(int A[], int &l) {
    scanf("%s", s);
    l = strlen(s);
    for(int i = 0; i < l; ++i)
        if(s[i] == '1') A[i] = 1;
        else A[i] = -1;
}

inline void init() {
    input(A, lA);
    input(B, lB);
}

inline double log(int a, int b) {
    return log(b) / log(a);
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

inline void DFT(cpx A[], int N, int flag) {
    for(int i = 0; i < N; ++i) {
        int p = 0, s = i;
        for(int j = 1; j < N; j <<= 1)
            p = p<<1|(s&1), s >>= 1;
        if(p > i) swap(A[i], A[p]);
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
            A[i] = A[i]/(double)N;
}

inline void work(int *A, int n, int id) {
    int N = 1;
    while(N <= n+lB) N <<= 1;
    for(int i = 0; i < n; ++i) T1[i].a = A[i], T1[i].b = 0;
    for(int i = n; i < N; ++i) T1[i].a = T1[i].b = 0;
    for(int i = 0; i < n-i-1; ++i) swap(T1[i], T1[n-i-1]);
    for(int i = 0; i < lB; ++i) T2[i].a = B[i], T2[i].b = 0;
    for(int i = lB; i < N; ++i) T2[i].a = T2[i].b = 0;
    DFT(T1, N, 1), DFT(T2, N, 1);
    for(int i = 0; i < N; ++i) T1[i] = T1[i]*T2[i];
    DFT(T1, N, -1);
    for(int i = 0; i < lB; ++i)
        sum[i][id] = (int)(n-T1[i].a+0.5)>>1;
}

inline void solve() {
    bS = sqrt(lA*log(2, lA));
    if(N > 10000) bS *= 2;
    bN = ceil(1.0*lA/bS);
    for(int i = 0; i < bN; ++i) {
        int l = bS*i, r = min(lA, bS*(i+1));
        work(A+l, r-l, i);
    }
    debug("%lfs\n", (double)clock()/CLOCKS_PER_SEC);
    int q;read(q);
    while(q--) {
        int l, r, len, ans = 0;
        read(l), read(r), read(len);
        int bl = l/bS, br = (l+len-1)/bS;
        if(bl == br) {
            for(int i = l; i < l+len; ++i)
                ans += A[i] != B[i-l+r];
            write(ans), putchar('\n');
            continue;
        }
        for(int i = l; i < (bl+1)*bS; ++i)
            ans += A[i] != B[i-l+r];
        for(int i = br*bS; i < l+len; ++i)
            ans += A[i] != B[i-l+r];
        for(int i = bl+1; i < br; ++i)
            ans += sum[(i+1)*bS-1-l+r][i];
        write(ans), putchar('\n');
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("G.in", "r", stdin);
    freopen("G.out", "w", stdout);
#endif

    init();
    solve();
    debug("%lfs\n", (double)clock()/CLOCKS_PER_SEC);
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
