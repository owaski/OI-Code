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
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
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

const int S = 7e5;

int p[S/10+9], pn;
bool is[S+9];

inline void euler(int lim) {
    for(int i = 2; i <= lim; ++i) {
        if(!is[i]) p[++pn] = i;
        for(int j = 1; j <= pn && 1ll*p[j]*i <= lim; ++j) {
            is[i*p[j]] = true;
            if(i%p[j] == 0) break;
        }
    }
}

int lim;
ll n;
ll x[S+9];
int xn;
ll g[S+9];
int upp[S+9];
int l[S+9];
ll f[S+9];

#define sqr(x) ((ll)(x)*(x))

inline int pos(ll x) {
    if(x <= lim) return x;
    else return xn-n/x+1;
}

inline void calcg() {
    for(int i = 1; i <= xn; ++i) g[i] = x[i];
    std::fill(l+1, l+xn+1, 0);
    for(int i = 1, k = 1; i <= pn; ++i) {
        ll t = sqr(p[i]);
        while(t > x[k] && k <= xn) ++k;
        for(int j = xn; j >= k; --j) {
            int d = pos(x[j]/p[i]);
            g[j] -= g[d]-(std::min(upp[d], i)-l[d]-1);
            l[j] = i;
        }
    }
    for(int i = 1; i <= xn; ++i) g[i] -= std::min(upp[i], pn+1)-l[i]-1;
    std::fill(g+1, g+lim+1, 0);
    for(int i = xn; i > lim; --i) g[i]--, g[i] *= 4;
    for(int i = 1; i <= xn; ++i) g[i]++;
}

inline void calcf() {
    std::fill(f+1, f+xn+1, 1);
    std::fill(l+1, l+xn+1, 0);
    for(int i = pn, k = xn; i >= 1; --i) {
        ll t = sqr(p[i]);
        while(k > 1 && t <= x[k-1]) --k;
        for(int j = xn; j >= k; --j) {
            ll pc = p[i];
            for(int c = 1; pc <= x[j]; ++c, pc *= p[i]) {
                int d = pos(x[j]/pc);
                f[j] += (3*c+1)*(f[d]+4*std::max(0, upp[d]-std::max(l[d], i)-1));
            }
            if(!l[j]) l[j] = i;
        }
    }
    for(int i = 1; i <= xn; ++i) f[i] += 4*std::max(0, upp[i]-l[i]-1);
    for(int i = xn; i >= 1; --i) f[i] -= f[i-1];
}

inline ll solve() {
    lim = xn = pn = 0;
    while(sqr(lim+1) <= n) ++lim;
    for(ll i = 1, j; i <= n; i = j) x[++xn] = (j=n/(n/i))++;
    while(p[pn+1] <= lim) ++pn;
    for(int i = 1, j = 1; i <= xn; ++i) {
        while(j <= pn && p[j] <= x[i]) ++j;
        upp[i] = j;
    }
    calcg(), calcf();ll ret = 0;
    for(int i = 1; i <= xn; ++i) ret += f[i]*g[xn-i+1];
    return ret;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("DIVCNT3.in", "r", stdin);
    freopen("DIVCNT3.out", "w", stdout);
#endif

    euler(S);
    int Case;
    read(Case);
    while(Case--) {
        read(n);
        write(solve());
        out[outn++] = '\n';
    }
    printf("%s", out);

#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
