#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 20009;
const int INF = 2e9;

int n;
int x[N], y[N];
int cov[N];

inline bool in(int a, int b, int c)
{
	return b <= a && a <= c;	
}

bool cover(int len, int d)
{
	bool flag = true;
	int lx = INF, ly = INF;
	int rx = -INF, ry = -INF;
	for(int i = 1; i <= n; ++i)
		if(!cov[i])
		{
			lx = min(lx, x[i]), rx = max(rx, x[i]);
			ly = min(ly, y[i]), ry = max(ry, y[i]);	
			flag = false;
		}
	if(!d) return flag;
	for(int i = 1; i <= n; ++i)
		if(in(x[i], lx, lx+len) && in(y[i], ly, ly+len))
			cov[i]++;
	if(cover(len, d-1)) return true;
	for(int i = 1; i <= n; ++i)
		if(in(x[i], lx, lx+len) && in(y[i], ly, ly+len))
			cov[i]--;
	for(int i = 1; i <= n; ++i)
		if(in(x[i], lx, lx+len) && in(y[i], ry-len, ry))
			cov[i]++;
	if(cover(len, d-1)) return true;
	for(int i = 1; i <= n; ++i)
		if(in(x[i], lx, lx+len) && in(y[i], ry-len, ry))
			cov[i]--;
	for(int i = 1; i <= n; ++i)
		if(in(x[i], rx-len, rx) && in(y[i], ly, ly+len))
			cov[i]++;
	if(cover(len, d-1)) return true;
	for(int i = 1; i <= n; ++i)
		if(in(x[i], rx-len, rx) && in(y[i], ly, ly+len))
			cov[i]--;
	for(int i = 1; i <= n; ++i)
		if(in(x[i], rx-len, rx) && in(y[i], ry-len, ry))
			cov[i]++;
	if(cover(len, d-1)) return true;
	for(int i = 1; i <= n; ++i)
		if(in(x[i], rx-len, rx) && in(y[i], ry-len, ry))
			cov[i]--;
	return false;
}

inline bool check(int len)
{
	memset(cov, 0, sizeof cov);
	return cover(len, 3);	
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1052.in", "r", stdin);
	freopen("bzoj1052.out", "w", stdout);
#endif

	scanf("%d", &n);
	int lx = INF, ly = INF;
	int rx = -INF, ry = -INF;
	for(int i = 1; i <= n; ++i)
	{
		scanf("%d%d", x+i, y+i);
		lx = min(lx, x[i]), rx = max(rx, x[i]);
		ly = min(ly, y[i]), ry = max(ry, y[i]);
	}
	int l = 0, r = max(rx-lx, ry-ly);
	while(l < r)
	{
		int mid = (l+r)>>1;
		if(check(mid)) r = mid;
		else l = mid+1;	
	}
	printf("%d\n", l);

	return 0;
}
