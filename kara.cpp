#include <bits/stdc++.h>

#define ll long long

#define pii std::pair<int,int>
#define mp std::make_pair
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
static int outn;
static char out[(int)2e7];
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

const int N = 500, S = N * N;
const int MOD = 998244353;

int n, s, r[N + 9];
int f[2][2][S + 9];

int fpm(int a, int b) {
    if(a <= 0) return 0;
    int ret = 1;
    while(b) {
        if(b & 1) ret = (ll)ret * a % MOD;
        a = (ll)a * a % MOD, b >>= 1;
    }
    return ret;
}

int main() {
    freopen("kara.in", "r", stdin);
    freopen("kara.out", "w", stdout);
    
    read(n), read(s);
    for(int i = 1; i <= n; ++i) read(r[i]);
    f[0][0][0] = 1;
    int sum = 0;
    for(int i = 0; i < n; sum += r[++i]) {
        int p = i & 1, q = p ^ 1;
        for(int j = 0; j < 2; ++j)
            for(int k = 0; k <= sum + r[i + 1]; ++k)
                f[q][j][k] = 0;
        for(int j = 0; j < 2; ++j)
            for(int k = 0; k <= sum; ++k)
                if(f[p][j][k]) {
                    (f[q][j][k] += f[p][j][k]) %= MOD;
                    (f[q][j ^ 1][k + r[i + 1]] += f[p][j][k]) %= MOD;
                }
    }
    int ans = 0;
    for(int i = 0; i <= sum; ++i) {
        if(f[n & 1][0][i]) (ans += (ll)fpm(s - i, n) * f[n & 1][0][i] % MOD) %= MOD;
        if(f[n & 1][1][i]) (ans += MOD - (ll)fpm(s - i, n) * f[n & 1][1][i] % MOD) %= MOD;
    }
    for(int i = 1; i <= n; ++i) ans = (ll)ans * fpm(i, MOD - 2) % MOD;
    printf("%d\n", ans);

    fclose(stdin);fclose(stdout);
    return 0;
}
