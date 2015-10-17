#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 209;

int n;
struct Ugraph
{
	int sz;
	int head[N<<1];
	int to[N*N];
	int ne[N*N];
	inline void init()
	{
		sz = 1;
		memset(head, 0, sizeof head);
	}
	inline void add_edge(int u, int v)
	{
		to[sz] = v, ne[sz] = head[u], head[u] = sz++;
	}
}G;
bool vis[N];
int mat[N];

bool dfs(int now)
{
	for(int i = G.head[now]; i; i = G.ne[i])
	{
		int to = G.to[i]-n;
		if(!vis[to])
		{
			vis[to] = true;
			if(!mat[to] || dfs(mat[to]))
			{
				mat[to] = now;
				return true;	
			}
		}
	}
	return false;
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1059.in", "r", stdin);
	freopen("bzoj1059.out", "w", stdout);
#endif

	int Case;
	scanf("%d", &Case);
	while(Case--)
	{
		G.init();
		memset(mat, 0, sizeof mat);
		scanf("%d", &n);
		for(int i = 1; i <= n; ++i)
			for(int j = 1; j <= n; ++j)
			{
				int x;
				scanf("%d", &x);
				if(x) G.add_edge(i, j+n);
			}
		int sum = 0;
		for(int i = 1; i <= n; ++i)
		{
			memset(vis, 0, sizeof vis);
			sum += dfs(i);
		}
		if(sum == n) puts("Yes");
		else puts("No");
	}

	return 0;
}
