#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
using namespace std;

const int MAXN = 159;
const int MAXK = 29;
const int MOD = 12345678;

int n, m, k;
int f[MAXN][MAXN][MAXK][MAXK];
int ans;

inline void add(int &x, const int &y)
{
	x += y;
	while(x >= MOD) x -= MOD;	
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1037.in", "r", stdin);
	freopen("bzoj1037.out", "w", stdout);
#endif

	scanf("%d%d%d", &n, &m, &k);
	f[0][0][0][0] = 1;
	rep(i, 0, n)
		rep(j, 0, m)
			rep(p, 0, k)
				rep(q, 0, k)
					if(p+q <= k && j-i <= p && i-j <= q)
					{
						int val = f[i][j][p][q];
						if(i-j == q && p+q < k)
							add(f[i+1][j][p][q+1], val);
						else if(i-j < q)
							add(f[i+1][j][p][q], val);
						if(j-i == p && p+q < k)
							add(f[i][j+1][p+1][q], val);
						else if(j-i < p)
							add(f[i][j+1][p][q], val);	
					}
	rep(i, 0, k)
		rep(j, 0, k)
			if(i+j <= k)
				add(ans, f[n][m][i][j]);
	printf("%d\n", ans);

	return 0;
}
