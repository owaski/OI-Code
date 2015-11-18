#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 509;
const int MOD = 1024523;

int n, m;
char s1[N], s2[N];
int f[N][N], g[N][N];
bool vis[N][N];
pair<int,int> q[N*N], q2[N*N];
int qTop, q2Top;

void init()
{
	scanf("%d%d", &n, &m);
	scanf("%s%s", s1+1, s2+1);
	reverse(s1+1, s1+n+1);
	reverse(s2+1, s2+m+1);
}

void add(int i, int p, int v)
{
	g[i][p] = (g[i][p]+v)%MOD;
	if(!vis[i][p])
	{
		vis[i][p] = true;
		q2[q2Top++] = make_pair(i, p);
	}
}

void solve()
{
	q[qTop++] = make_pair(0, 0), f[0][0] = 1;
	for(int l = 1; l <= n+m; ++l)
	{
		while(qTop--)
		{
			pair<int,int> cur = q[qTop];
			int i = cur.first, j = l-i-1;
			int p = cur.second, qq = l-p-1;
			int val = f[i][p];f[i][p] = 0;
			if(i < n && p < n && s1[i+1] == s1[p+1]) add(i+1, p+1, val);
			if(i < n && qq < m && s1[i+1] == s2[qq+1]) add(i+1, p, val);
			if(j < m && qq < m && s2[j+1] == s2[qq+1]) add(i, p, val);
			if(j < m && p < n && s1[p+1] == s2[j+1]) add(i, p+1, val);
		}
		qTop = 0;
		while(q2Top--)
		{
			pair<int,int> cur = q2[q2Top];
			int i = cur.first, p = cur.second;
			f[i][p] = g[i][p], q[qTop++] = cur;
			g[i][p] = 0, vis[i][p] = false;
		}
		q2Top = 0;
	}
	cout << f[n][n] << endl;
}

int main()
{
	freopen("ballb.in", "r", stdin);
	freopen("ballb.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
