#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

template<class T>
inline void read(T &x)
{
	char c;x = 0;
	while((c=getchar()) < '0' || c > '9');
	x = c-'0';
	while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
}

const int N = 2009;
const int INF = 0x3f3f3f3f;

int n, m;
int g[N][N];
int le[N][N];
int up[N][N];
int ri[N][N];
int ans1, ans2;

inline int sqr(int x)
{
	return x*x;
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1057.in", "r", stdin);
	freopen("bzoj1057.out", "w", stdout);
#endif

	read(n), read(m);
	memset(g, -1, sizeof g);
	for(int i = 1; i <= n; ++i)
		for(int j = 1; j <= m; ++j)
			read(g[i][j]);
	for(int i = 1; i <= n; ++i)
		for(int j = 1; j <= m; ++j)
			if(g[i][j] != g[i][j-1])
				le[i][j] = le[i][j-1]+1;
			else le[i][j] = 1;
	for(int i = 1; i <= n; ++i)
		for(int j = m; j >= 1; --j)
			if(g[i][j] != g[i][j+1])
				ri[i][j] = ri[i][j+1]+1;
			else ri[i][j] = 1;
	memset(le[0], INF, sizeof le[0]);
	memset(ri[0], INF, sizeof ri[0]);
	for(int j = 1; j <= m; ++j)
		for(int i = 1; i <= n; ++i)
		{
			if(g[i][j] != g[i-1][j])
			{
				up[i][j] = up[i-1][j]+1;
				le[i][j] = min(le[i][j], le[i-1][j]);
				ri[i][j] = min(ri[i][j], ri[i-1][j]);
			}
			else up[i][j] = 1;
			ans1 = max(ans1, sqr(min(up[i][j], le[i][j]+ri[i][j]-1)));
			ans2 = max(ans2, up[i][j]*(le[i][j]+ri[i][j]-1));
		}
	printf("%d\n%d\n", ans1, ans2);

	return 0;
}
