#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 69;

int n, m;
double k;
int fa[N];
struct Tree
{
	int sz;
	int head[N];
	int to[N<<1];
	int ne[N<<1];
	Tree()
	{
		sz = 1;
		memset(head, 0, sizeof head);
	}
	inline void add_edge(int u, int v)
	{
		to[sz] = v, ne[sz] = head[u], head[u] = sz++;
	}
}G;
double c[N];
int cirLen;

bool del[N];
bool vis[N];
double f[N][N][N];
double p[N], tp[N];
double g[N][N], add;
double Pow[N];
double ans;

inline void init()
{
	scanf("%d%d%lf", &n, &m, &k);
	for(int i = 1; i <= n; ++i)
	{
		scanf("%d", fa+i);
		G.add_edge(fa[i], i);
	}
	for(int i = 1; i <= n; ++i)
		scanf("%lf", c+i);
}

inline void getCirLen()
{
	int now = fa[1];
	cirLen = 1;
	while(now != 1)
	{
		cirLen++;
		now = fa[now];
	}
}

void dp(int now, int dep)
{
	vis[now] = true;
	for(int i = G.head[now]; i; i = G.ne[i])
		if(!vis[G.to[i]] && !del[i]) dp(G.to[i], dep+1);
	if(now == 3)
		now--,now++;
	if(now == 1)
	{
		add = c[1];
		f[1][0][0] = add;
		for(int j = G.head[now]; j; j = G.ne[j])
			if(!del[j])
			{
				memcpy(g, f[1], sizeof g);
				for(int i = m; i >= 0; --i)
					for(int k = 0; k <= i; ++k)
						f[1][0][i] = max(f[1][0][i], g[0][i-k]+f[G.to[j]][1][k]);
			}
	}
	else 
	{
		memset(p, 0, sizeof p);
		p[0] = c[now]*k;
		for(int i = 1; i <= dep; ++i)
		{
			add = c[now]*Pow[i];
			if((i < dep && m && i > 1) || i == dep)
				f[now][i][0] = add;
		}
		for(int j = G.head[now]; j; j = G.ne[j])
			if(!del[j])
			{
				int to = G.to[j];
				memcpy(g, f[now], sizeof g);
				memcpy(tp, p, sizeof tp);
				for(int i = m; i >= 0; --i)
					for(int k = 0; k <= i; ++k)
					{
						p[i] = max(p[i], tp[i-k]+max(f[to][1][k], f[to][2][k]));
						for(int d = dep>1?2:1; d <= dep; ++d)
							f[now][d][i] = max(f[now][d][i], g[d][i-k]+max(f[to][d+1][k], f[to][1][k]));
					}
			}
		for(int i = 1; i <= m; ++i)
			f[now][1][i] = max(f[now][1][i], p[i-1]);
	}
}

inline void solve()
{
	Pow[0] = 1;
	for(int i = 1; i <= n; ++i)
		Pow[i] = Pow[i-1]*k;
	getCirLen();
	for(int i = fa[1], l = 2; i != 1; i = fa[i], ++l)
	{
		memset(f, 0, sizeof f);
		memset(vis, false, sizeof vis);
		del[i] = true, G.add_edge(1, i);
		dp(1, 0), del[G.sz-1] = true, del[i] = false;
		if(m || fa[i] == 1)
		{
			double tmp = f[1][0][m-(fa[i]!=1)];
			ans = max(ans, tmp/(1-Pow[l]));
		}
	}
	printf("%.2lf\n", ans);
}


int main()
{
	freopen("bzoj1065.in", "r", stdin);
	freopen("bzoj1065.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
