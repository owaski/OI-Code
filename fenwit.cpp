#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

#define ll long long

template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c= getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}
int outn;
char out[10000000];
template<class T>
inline void write(T x) {
    if(x < 0) out[outn++] = '-', x = -x;
    if(x) {
        int tmpn = 0;
        char tmp[20] = {0};
        while(x) tmp[tmpn++] = x%10+'0', x /= 10;
        while(tmpn) out[outn++] = tmp[--tmpn];
    }
    else out[outn++] = '0';
}

const int M = 18;
const int L = 1000;

int m;
ll MOD, T, phiMOD;
long double TT;
char str[L+9];
int b[M+9];
ll F[1<<M], B[1<<M];

inline ll getphi(ll x) {
    ll ret = x;
    for(ll i = 2; i*i <= x; ++i)
        if(x%i == 0) {
            ret = ret/i*(i-1);
            while(x%i == 0)
                x /= i;
        }
    if(x) ret = ret/x*(x-1);
    return ret;
}

inline void FWT(ll *F, int n, int f) {
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < (1<<n); ++j)
            if(!(j&(1<<i))) {
                ll u = F[j], v = F[j|1<<i];
                F[j] = (u+v)%MOD, F[j|1<<i] = (u-v+MOD)%MOD;
            }
    if(f == -1) {
        for(int i = 0; i < (1<<n); ++i)
            F[i] >>= n;
    }
}

inline ll mult(ll a, ll b) {
    return (a*b-(long long)((long double)a*b/MOD)*MOD+MOD)%MOD;
}

inline ll fpm(ll a, ll b) {
    ll ret = 1;
    while(b) {
        if(b&1) ret = mult(ret, a);
        a = mult(a, a), b >>= 1;
    }
    return ret;
}

int main() {
    freopen("fenwit.in", "r", stdin);
    freopen("fenwit.out", "w", stdout);

    std::cin >> m >> MOD >> str;
    MOD <<= m, phiMOD = getphi(MOD);
    for(int i = 0, l = strlen(str); i < l; ++i)
        T = (T*10+str[i]-'0')%phiMOD, TT = TT*10+str[i]-'0';
    if(TT >= phiMOD) T += phiMOD;
    for(int i = 0; i < (1<<m); ++i) read(F[i]);
    for(int i = 0; i <= m; ++i) read(b[i]);
    for(int i = 0; i < (1<<m); ++i) {
        int c = 0, t = i;
        while(t) c += t&1, t >>= 1;
        B[i] = b[c];
    }
    FWT(F, m, 1), FWT(B, m, 1);
    for(int i = 0; i < (1<<m); ++i)
        F[i] = mult(F[i], fpm(B[i], T));
    FWT(F, m, -1);
    for(int i = 0; i < (1<<m); ++i)
        write(F[i]), out[outn++] = '\n';
    printf("%s", out);
    
    fclose(stdin);fclose(stdout);
    return 0;
}
