#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 600009;
const char c[] = "ATCG";
const double pi = acos(-1.0);

int lS, lT, k;
char S[N], T[N];
int tag[N];
int sum[N];
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
    inline friend cpx operator / (const cpx &p, const double k) {
        return cpx(p.a/k, p.b/k);
    }
}A[N], B[N];

inline void init() {
    cin >> lS >> lT >> k;
    scanf("%s%s", S, T);
}                

inline int pre(int x) {
    return max(0, x-k);
}
inline int suf(int x) {
    return min(lS, x+k+1);
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
            A[i] = A[i]/N;
}

inline void work(char c) {
    int N = 1;
    while(N <= lS+lT) N <<= 1;
    for(int i = 0; i < N; ++i)
        A[i].a = A[i].b = B[i].a = B[i].b = 0;
    for(int i = 0; i < lS; ++i)
        if(S[i] == c)
            tag[pre(i)]++, tag[suf(i)]--;
    for(int i = 0; i < lT; ++i)
        B[i].a = T[lT-i-1] == c;
    int flag = 0;
    for(int i = 0; i < lS; ++i) {
        flag += tag[i];
        A[i].a = flag > 0;
        tag[i] = 0;
    }
    DFT(A, N, 1), DFT(B, N, 1);
    for(int i = 0; i < N; ++i) A[i] = A[i]*B[i];
    DFT(A, N, -1);
    for(int i = 0; i < lS; ++i) sum[i] += (int)(A[i].a+0.5);
}

inline void solve() {
    int ans = 0;
    for(int i = 0; i < 4; ++i)
        work(c[i]);
    for(int i = 0; i < lS; ++i)
        ans += sum[i] == lT;
    cout << ans << endl;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("D.in", "r", stdin);
    freopen("D.out", "w", stdout);
#endif

    init();
    solve();
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
