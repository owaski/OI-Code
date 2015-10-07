#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

int a, b, n;
double g[11][11], avg;
double sum[11][11];
double f[11][11][11][11][11];
bool v[11][11][11][11][11];

inline double sqr(double x)
{
	return x*x;	
}

inline double S(int x1, int y1, int x2, int y2)
{
	return sum[x2][y2]-sum[x1-1][y2]-sum[x2][y1-1]+sum[x1-1][y1-1];
}

double dp(int x1, int y1, int x2, int y2, int n)
{
	if(v[x1][y1][x2][y2][n])
		return f[x1][y1][x2][y2][n];
	v[x1][y1][x2][y2][n] = true;
	if(n == 1)
		return f[x1][y1][x2][y2][n] = sqr(S(x1, y1, x2, y2)-avg);
	double ret = 1e15;
	for(int i = x1; i < x2; ++i)
		for(int j = 1; j < n; ++j)
			ret = min(ret, dp(x1, y1, i, y2, j)+dp(i+1, y1, x2, y2, n-j));
	for(int i = y1; i < y2; ++i)
		for(int j = 1; j < n; ++j)
			ret = min(ret, dp(x1, y1, x2, i, j)+dp(x1, i+1, x2, y2, n-j));
	return f[x1][y1][x2][y2][n] = ret;
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1048.in", "r", stdin);
	freopen("bzoj1048.out", "w", stdout);
#endif

	scanf("%d%d%d", &a, &b, &n);
	for(int i = 1; i <= a; ++i)
		for(int j = 1; j <= b; ++j)
		{
			scanf("%lf", &g[i][j]);
			avg += g[i][j], sum[i][j] = sum[i][j-1]+sum[i-1][j]-sum[i-1][j-1]+g[i][j];
		}
	avg /= n;
	printf("%.2lf\n", sqrt(dp(1, 1, a, b, n)/n));

	return 0;
}
