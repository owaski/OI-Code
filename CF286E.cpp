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

const int N = 3e6+9;
const double pi = acos(-1.0);
const double eps = 1e-3;

int n, m;
int F[N];
struct cpx {
    double a, b;
    cpx(double _a = 0, double _b = 0):a(_a), b(_b) {}
    inline friend cpx operator + (const cpx &p, const cpx &q) {
        return cpx(p.a+q.a, p.b+q.b);
    }
    inline friend cpx operator - (const cpx &p, const cpx &q) {
        return cpx(p.a-q.a, p.b-q.b);
    }
    inline friend cpx operator * (const cpx &p, const cpx &q) {
        return cpx(p.a*q.a-p.b*q.b, p.a*q.b+p.b*q.a);
    }
    inline friend cpx operator / (const cpx &p, const double &q) {
        return cpx(p.a/q, p.b/q);
    }
}A[N];
int ans[N], ansN;

inline void init() {
    read(n), read(m);
    for(int i = 1; i <= n; ++i) {
        int x;read(x);
        F[x] = A[x].a = 1;
    }
}

inline void FFT(cpx A[], int N, int flag) {
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
            A[i] = A[i]/n;
}

inline bool isZero(const double &x) {
    return fabs(x) <= eps;
}

inline void solve() {
    int N = 1;
    while(N < 2*m+1) N <<= 1;
    FFT(A, N, 1);
    for(int i = 0; i < N; ++i)
        A[i] = A[i]*A[i];
    FFT(A, N, -1);
    for(int i = 1; i <= m; ++i)
        if(F[i] && isZero(A[i].a))
            ans[++ansN] = i;
        else if(!F[i] && !isZero(A[i].a)) {
            puts("NO");
            return ;
        }
    puts("YES");
    write(ansN), putchar('\n');
    for(int i = 1; i <= ansN; ++i)
        write(ans[i]), putchar(i==ansN?'\n':' ');
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
