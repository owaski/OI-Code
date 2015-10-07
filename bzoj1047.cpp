#include <bits/stdc++.h>
using namespace std;

template<class T>
inline void read(T &x)
{
	char c;int f = 1;x = 0;
	while(((c=getchar()) < '0' || c > '9') && c != '-');
	if(c == '-') f = -1;else x = c-'0';
	while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
	x *= f;	
}

const int MAXN = 1009;
const int INF = 1e9;

int a, b, n;
int g[MAXN][MAXN];
int minv[MAXN][MAXN];
int maxv[MAXN][MAXN];
int qmin[MAXN], lmin, rmin;
int qmax[MAXN], lmax, rmax;
int ans = INF;

inline void get_max()
{
	for(int i = 1; i <= a; ++i)
	{
		lmin = lmax = 0, rmin = rmax = -1;
		for(int j = 1; j <= n; ++j)
		{
			while(lmin <= rmin && g[i][qmin[rmin]] >= g[i][j]) --rmin;
			while(lmax <= rmax && g[i][qmax[rmax]] <= g[i][j]) --rmax;
			qmin[++rmin] = qmax[++rmax] = j;
		}
		minv[i][1] = g[i][qmin[lmin]], maxv[i][1] = g[i][qmax[lmax]];
		for(int j = n+1; j <= b; ++j)
		{
			while(lmin <= rmin && j-qmin[lmin] >= n) lmin++;
			while(lmax <= rmax && j-qmax[lmax] >= n) lmax++;
			while(lmin <= rmin && g[i][qmin[rmin]] >= g[i][j]) --rmin;
			while(lmax <= rmax && g[i][qmax[rmax]] <= g[i][j]) --rmax;
			qmin[++rmin] = qmax[++rmax] = j;
			minv[i][j-n+1] = g[i][qmin[lmin]];
			maxv[i][j-n+1] = g[i][qmax[lmax]];
		}
	}
}

inline void solve(int j)
{
	lmax = lmin = 0, rmax = rmin = -1;
	for(int i = 1; i <= n; ++i)
	{
		while(lmin <= rmin && minv[qmin[rmin]][j] >= minv[i][j]) --rmin;
		while(lmax <= rmax && maxv[qmax[rmax]][j] <= maxv[i][j]) --rmax;
		qmin[++rmin] = qmax[++rmax] = i;
	}
	ans = min(ans, maxv[qmax[lmax]][j]-minv[qmin[lmin]][j]);
	for(int i = n+1; i <= a; ++i)
	{
		while(lmin <= rmin && i-qmin[lmin] >= n) ++lmin;
		while(lmax <= rmax && i-qmax[lmax] >= n) ++lmax;
		while(lmin <= rmin && minv[qmin[rmin]][j] >= minv[i][j]) --rmin;
		while(lmax <= rmax && maxv[qmax[rmax]][j] <= maxv[i][j]) --rmax;
		qmin[++rmin] = qmax[++rmax] = i;
		ans = min(ans, maxv[qmax[lmax]][j]-minv[qmin[lmin]][j]);
	}
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1047.in", "r", stdin);
	freopen("bzoj1047.out", "w", stdout);	
#endif

	read(a), read(b), read(n);
	for(int i = 1; i <= a; ++i)
		for(int j = 1; j <= b; ++j)
		{
			read(g[i][j]);
			maxv[i][j] = -INF;
			minv[i][j] = INF;
		}
	get_max();
	for(int i = 1; i <= b-n+1; ++i)
		solve(i);
	cout << ans << endl;

	return 0;
}
