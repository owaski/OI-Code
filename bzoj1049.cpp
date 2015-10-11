#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 35009;
const int INF = 0x3f3f3f3f;

int n;
int a[N];
int maxl, Min[N];
int f[N];
vector<int> cnt[N];
long long p1[N];
long long p2[N];
long long g[N];
int ans;

inline void solve1()
{
	memset(Min, INF, sizeof Min);
	Min[0] = -INF, cnt[0].push_back(0);
	for(int i = 1; i <= n; ++i)
	{
		int p = upper_bound(Min, Min+maxl+1, a[i])-Min;
		f[i] = p, maxl = max(maxl, p), Min[p] = a[i];
		cnt[p].push_back(i);
	}
}

inline void solve2()
{
	memset(g, INF, sizeof g);g[0] = 0;
	for(int i = 1; i <= n; ++i)
		for(int j = 0; j < (int)cnt[f[i]-1].size(); ++j)
		{
			int p = cnt[f[i]-1][j];
			if(p > i) break;
			if(a[p] > a[i]) continue;
			for(int k = p; k <= i; ++k)
			{
				p1[k] = abs(a[k]-a[p]);
				p2[k] = abs(a[k]-a[i]);	
			}
			for(int k = p+1; k <= i; ++k)
				p1[k] += p1[k-1], p2[k] += p2[k-1];
			for(int k = p; k <= i; ++k)
				g[i] = min(g[i], g[p]+p1[k]-p1[p]+p2[i]-p2[k]);
		}
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1049.in", "r", stdin);
	freopen("bzoj1049.out", "w", stdout);
#endif

	scanf("%d", &n);a[0] = -INF;
	for(int i = 1; i <= n; ++i)
	{
		scanf("%d", a+i);
		a[i] -= i;
	}
	a[++n] = INF;
	solve1();
	cout << n-maxl << endl;
	solve2();
	cout << g[n] << endl;

	return 0;
}
