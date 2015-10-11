#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 10009;
const int M = 50009;

int n, m;
struct Dgraph
{
	int sz;
	int head[N];
	int to[M];
	int ne[M];
	Dgraph()
	{
		sz = 1;
		memset(head, 0, sizeof head);
	}
	inline void add_edge(int u, int v)
	{
		to[sz] = v, ne[sz] = head[u], head[u] = sz++;
	}
}G, D;

int dfn[N];
int low[N];
int time_f;
stack<int> s;
bool in[N];
int col[N], tot;
int deg[N];
queue<int> q;
bool vis[N];

void dfs(int now)
{
	s.push(now), in[now] = true;
	dfn[now] = low[now] = ++time_f;
	for(int i = G.head[now]; i; i = G.ne[i])
		if(!dfn[G.to[i]])
		{
			dfs(G.to[i]);
			low[now] = min(low[now], low[G.to[i]]);
		}
		else if(in[G.to[i]])
			low[now] = min(low[now], dfn[G.to[i]]);
	if(dfn[now] == low[now])
	{
		++tot;int p;
		do
		{
			p = s.top();
			s.pop(), col[p] = tot;	
			in[p] = false;
		}while(p != now);
	}
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1051.in", "r", stdin);
	freopen("bzoj1051.out", "w", stdout);
#endif

	scanf("%d%d", &n, &m);
	for(int i = 1; i <= m; ++i)
	{
		int u, v;
		scanf("%d%d", &u, &v);
		G.add_edge(u, v);
	}
	for(int i = 1; i <= n; ++i)
		if(!dfn[i])
			dfs(i);
	for(int i = 1; i <= n; ++i)
		for(int j = G.head[i]; j; j = G.ne[j])
			if(col[i] != col[G.to[j]])
			{
				D.add_edge(col[G.to[j]], col[i]);
				deg[col[i]]++;
			}
	int root = 0, sum = 0;
	for(int i = 1; i <= tot; ++i)
		if(!deg[i])
		{
			if(root)
			{
				puts("0");
				return 0;	
			}
			root = i;
			q.push(i);
			sum = 1;
		}
	while(!q.empty())
	{
		int now = q.front();q.pop();
		for(int i = D.head[now]; i; i = D.ne[i])
			if(!vis[D.to[i]])
			{
				vis[D.to[i]] = true;
				sum++, q.push(D.to[i]);
			}
	}
	if(sum < tot) puts("0");
	else 
	{
		int ans = 0;
		for(int i = 1; i <= n; ++i)
			if(col[i] == root)
				ans++;
		printf("%d\n", ans);	
	}
	
	return 0;
}
