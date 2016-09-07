#include <bits/stdc++.h>

#define ll long long
#define uint unsigned

#define debug(...) fprintf(stderr, __VA_ARGS__)

#define SZ(x) ((int)(x).size())
#define pb push_back

#define pii std::pair<int,int>
#define mp std::make_pair
#define fi first
#define se second

template<class T>inline void chkmax(T &x, const T &y) {if(x < y) x = y;}
template<class T>inline void chkmin(T &x, const T &y) {if(y < x) x = y;}

template<class T>
inline void read(T &x) {
	char c;int f = 1;x = 0;
	while(((c = getchar()) < '0' || c > '9') && c != '-');
	if(c == '-') f = -1;else x = c - '0';
	while((c = getchar()) >= '0' && c <= '9') x = x * 10 + c - '0';
	x *= f;
}

const int MOD = 1e9 + 7;
const int N = 1500, K = 1e5;

int n, m;
int a, b, p;
int k;

int fac[K + 9], inv[K + 9];
int pw[K + 9], ipw[K + 9];
int w[N + 9], prew[N + 9];

int f[N + 9][N + 9], s[N + 9], g[N + 9];

int fpm(int a, int b) {
	int ret = 1;
	while(b) {
		if(b & 1) ret = (ll)ret * a % MOD;
		a = (ll)a * a % MOD, b >>= 1;
	}
	return ret;
}

int C(int n, int m) {
	return (ll)fac[n] * inv[m] % MOD * inv[n - m] % MOD;
}

int main() {
#ifndef ONLINE_JUDGE
	freopen("E.in", "r", stdin);
	freopen("E.out", "w", stdout);
#endif

	read(n), read(m);
	read(a), read(b), p = (ll)a * fpm(b, MOD - 2) % MOD;
	read(k);

	fac[0] = 1;
	for(int i = 1; i <= k; ++i) fac[i] = (ll)fac[i - 1] * i % MOD;
	inv[k] = fpm(fac[k], MOD - 2);
	for(int i = k - 1; i >= 0; --i) inv[i] = (ll)inv[i + 1] * (i + 1) % MOD;

	pw[0] = ipw[0] = 1;
	for(int i = 1; i <= k; ++i) pw[i] = (ll)pw[i - 1] * p % MOD, ipw[i] = (ll)ipw[i - 1] * (1 - p + MOD) % MOD;

	for(int i = 0; i <= m; ++i) {
		if(i <= k) w[i] = (ll)C(k, i) * pw[i] % MOD * ipw[k - i] % MOD;
		prew[i] = (prew[i - 1] + w[i]) % MOD;
	}

	f[0][m] = 1;
	for(int i = 1; i <= n; ++i) {
		for(int j = 1; j <= m; ++j) s[j] = (s[j - 1] + f[i - 1][j]) % MOD, g[j] = (g[j - 1] + (ll)w[j] * s[j] % MOD) % MOD;
		for(int j = 1; j <= m; ++j) f[i][j] = ((ll)(s[m] - s[m - j] + MOD) % MOD * prew[j - 1] % MOD - g[j - 1] + MOD) % MOD * w[m - j] % MOD;
	}
	
	int ans = 0;
	for(int i = 1; i <= m; ++i) (ans += f[n][i]) %= MOD;
	std::cout << ans << std::endl;
	
	return 0;
}
