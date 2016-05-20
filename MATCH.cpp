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

const int N = 5;
const int M = 100;
const int S = 500;

int n, m;
double p[N+9][M+9];
double P[1<<N];
int setn;
std::map<unsigned,int> id;
int idn;
unsigned hall[S+9];
double f[M+9][S+9];

inline int get(unsigned x) {
    if(id.count(x)) {
        return id[x];
    } else {
        return hall[++idn] = x, id[x] = idn;
    }
}

inline double getP(int i, int s) {
    double ret = 1;
    for(int j = 0; j < n; ++j) {
        if(s & (1<<j)) {
            ret *= p[j+1][i];
        } else {
            ret *= 1-p[j+1][i];
        }
    }
    return ret;
}

inline int maxmatch(int x) {
    unsigned s = hall[x];int ret = 0;
    for(unsigned i = s; i; i -= i&-i) {
        int j = __builtin_ctz(i&-i);
        chkmax(ret, __builtin_popcount(j));
    }
    return ret;
}

inline unsigned move(unsigned s, int p) {
    unsigned ret = s;
    for(int i = 0; i < setn; ++i) {
        if(s & (1u<<i)) {
            for(int j = 0; j < n; ++j) {
                if(p & (1<<j)) {
                    ret |= 1u<<(i|(1<<j));
                }
            }
        }
    }
    //printf("(%u-%d)->%u\n", s, p, ret);
    return ret;
}

int main() {
    read(n), read(m);
    for(int i = 1; i <= n; ++i) {
        for(int j = 1; j <= m; ++j) {
            scanf("%lf", &p[i][j]);
        }
    }
    setn = 1<<n;
    f[0][get(1)] = 1;
    for(int i = 0; i < m; ++i) {
        memset(P, 0, sizeof P);
        for(int j = 0; j < setn; ++j)
            P[j] = getP(i+1, j);
        for(int j = idn; j >= 1; --j) {
            double v = f[i][j];
            if(!v) continue;
            for(int k = 0; k < setn; ++k) {
                f[i+1][get(move(hall[j], k))] += v*P[k];
            }
        }
    }
    double ans = 0;
    // for(int i = 1; i <= idn; ++i)
    //     printf("%lf\n", f[m][i]);
    for(int i = 1; i <= idn; ++i) {
        ans += f[m][i]*maxmatch(i);
    }
    printf("%.10lf\n", ans);
    return 0;
}
