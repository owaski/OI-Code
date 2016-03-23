#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

// m = 5
// 1 1 1 1 1
// 1 0 0 0 0
// 0 1 0 0 0
// 0 0 1 0 0
// 0 0 0 1 0

const int N = 500;
const int M = 6;
const int MOD = 998244353;

int n, m;
char str[N+9];

struct matrix {
    int n, m;
    int g[M][M];
    matrix() {
        n = m = 0;
        memset(g, 0, sizeof g);
    }
    inline void set(int _m) {
        n = m = _m;
        for(int i = 1; i <= m; ++i)
            g[1][i] = 1;
        for(int i = 2; i <= m; ++i)
            g[i][i-1] = 1;
    }
    inline void unit(int _m) {
        n = m = _m;
        for(int i = 1; i <= n; ++i)
            g[i][i] = 1;
    }
    inline friend matrix operator * (const matrix &p, const matrix &q) {
        matrix r;
        r.n = p.n, r.m = q.m;
        for(int i = 1; i <= r.n; ++i)
            for(int j = 1; j <= r.m; ++j)
                for(int k = 1; k <= p.m; ++k)
                    (r.g[i][j] += 1ll*p.g[i][k]*q.g[k][j]%MOD) %= MOD;
        return r;
    }
    inline void operator *= (const matrix &p) {
        *this = *this*p;
    }
    inline friend matrix operator + (const matrix &p, const matrix &q) {
        matrix r;
        r.n = p.n, r.m = q.m;
        for(int i = 1; i <= r.n; ++i)
            for(int j = 1; j <= r.m; ++j)
                r.g[i][j] = (p.g[i][j]+q.g[i][j]) % MOD;
        return r;
    }
    inline void operator += (const matrix &p) {
        *this = *this+p;
    }
    inline void debug() {
        printf("n = %d, m = %d\n", n, m);
        for(int i = 1; i <= n; ++i)
            for(int j = 1; j <= m; ++j)
                printf("%d%c", g[i][j], j==m?'\n':' ');
    }
}trans, p[N+9][10], f[N+9];

inline matrix pow10(const matrix &p) {
    matrix ret;ret.unit(m);
    for(int i = 1; i <= 10; ++i)
        ret *= p;
    return ret;
}

int main() {
    scanf("%s%d", str+1, &m);
    n = strlen(str+1);
    trans.set(m), p[0][0].unit(m);
    for(int i = 1; i < 10; ++i)
        p[0][i] = p[0][i-1]*trans;
    for(int i = 1; i < n; ++i)
        for(int j = 0; j < 10; ++j)
            p[i][j] = pow10(p[i-1][j]);
    f[0].unit(m);
    for(int i = 1; i <= n; ++i) {
        matrix now;
        f[i].n = f[i].m = m, now.unit(m);
        for(int j = i-1; j >= 0; --j) {
            now *= p[i-j-1][str[j+1]-'0'];
            f[i] += f[j]*now;
        }
    }
    printf("%d\n", f[n].g[1][1]);
    return 0;
}
