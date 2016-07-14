#include <bits/stdc++.h>

#define ll long long

#define pii std::pair<int,int>
#define mp std::make_pair
#define fi first
#define se second

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

const int N = 2000, N2 = 1000000;
const int MOD = 1e9 + 7;

int n, k;
int a[N2 + 9], b[N2 + 9], p[N2 + 9];
int f[N + 9][N + 9], c[N + 9][N + 9];
int g[N + 9], fac[N2 + 9], ans;

int fpm(int a, int b) {
    int ret = 1;
    while(b) {
        if(b & 1) ret = (ll)ret * a % MOD;
        a = (ll)a * a % MOD, b >>= 1;
    }
    return ret;
}

int main() {
    freopen("duel.in", "r", stdin);
    freopen("duel.out", "w", stdout);

    read(n), read(k);
    fac[0] = 1;
    for(int i = 1; i <= n; ++i) fac[i] = (ll)fac[i - 1] * i % MOD;
    for(int i = 1; i <= n; ++i) read(a[i]);
    for(int i = 1; i <= n; ++i) read(b[i]);
    for(int i = 1; i <= n; ++i) {
        p[i] = p[i - 1];
        while(p[i] < n && b[p[i] + 1] <= a[i]) ++p[i];
    }
    if(k == 1) {
        for(int i = 1; i <= n; ++i) (ans += p[i]) %= MOD;
        ans = (ll)ans * fpm(n, MOD - 2) % MOD;
        printf("%d\n", ans);
    }
    else if(k == 2) {
        int sum = 0;
        for(int i = 1; i <= n; ++i) (sum += p[i]) %= MOD;
        ans = (ll)sum * fpm(n, MOD - 2) % MOD, sum = (ll)sum * sum % MOD;
        for(int i = 1; i <= n; ++i) {
            (sum += MOD - (ll)p[i] * p[i] % MOD) %= MOD;
            (sum += MOD - (ll)p[i] * (n - i) * 2 % MOD) %= MOD;
        }
        (ans += (ll)sum * fpm(n - 1, MOD - 2) % MOD * fpm(n, MOD - 2) % MOD) %= MOD;
        printf("%d\n", ans);
    }
    else {
        c[0][0] = 1;
        for(int i = 1; i <= n; ++i)
            for(int j = 0; j <= i; ++j) {
                c[i][j] = c[i - 1][j];
                if(j) (c[i][j] += c[i - 1][j - 1]) %= MOD;
            }
        f[0][0] = 1;
        for(int i = 1; i <= n; ++i)
            for(int j = 0; j <= p[i]; ++j) {
                f[i][j] = f[i - 1][j];
                if(j) (f[i][j] += (ll)f[i - 1][j - 1] * (p[i] - (j - 1)) % MOD) %= MOD;
            }
        for(int i = p[n]; i; --i) {
            g[i] = (ll)f[n][i] * fac[n - i] % MOD;
            for(int j = i + 1; j <= p[n]; ++j)
                (g[i] += MOD - (ll)c[j][i] * g[j] % MOD) %= MOD;
            (ans += (ll)fpm(i, k) * g[i] % MOD) %= MOD;
        }
        ans = (ll)ans * fpm(fac[n], MOD - 2) % MOD;
        printf("%d\n", ans);
    }
	
    fclose(stdin);fclose(stdout);
    return 0;
}
