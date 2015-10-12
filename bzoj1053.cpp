#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

int n;
int prime[15] = {0, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43};
int g[15];
long long f[15][40];
int maxd, ans;

void dfs(int now, int lim, int d, int num)
{
	if(now == 14)
	{
		if(maxd < d)
		{
			maxd = d;
			ans = num;
		}
		else if(maxd == d)
			ans = min(ans, num);
		return ;
	}
	for(int i = min(g[now], lim); i >= 0; --i)
		if(f[now][i]*num <= n)
			dfs(now+1, i, d*(i+1), f[now][i]*num);
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1053.in", "r", stdin);
	freopen("bzoj1053.out", "w", stdout);
#endif

	cin >> n;
	for(int i = 1; i <= 14; ++i)
	{
		long long p = prime[i], base = 1;
		f[i][0] = 1;
		while(base*p <= n)
		{
			base *= p;
			f[i][++g[i]] = base;
		}
	}
	for(int i = 0; i <= g[1]; ++i)
		dfs(1, i, 1, 1);
	cout << ans << endl;

	return 0;
}
