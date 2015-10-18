#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int MAXN = 100009;
const int MAXM = 200009;

int n, m;
int Case;

namespace Graph
{
	struct UGraph
	{
		int sz;
		int head[MAXN];
		int to[MAXM<<1];
		int ne[MAXM<<1];
		inline void init()
		{
			sz = 1;
			memset(head, 0, sizeof head);	
		}
		inline void add_edge(int u, int v)
		{
			to[sz] = v, ne[sz] = head[u], head[u] = sz++;
			to[sz] = u, ne[sz] = head[v], head[v] = sz++;
		}
	}G, T;
	int time_f;
	int dfn[MAXN];
	int low[MAXN];
	int count;
	int bel[MAXN], tot;
	bool is[MAXM];
	pair<int,int> bridge[MAXN];
	inline int id(int i)
	{
		return (i+1)>>1;	
	}
	inline int inv(int i)
	{
		return ((i-1)^1)+1;	
	}
	inline void init()
	{
		G.init(), T.init();
		time_f = tot = count = 0;
		memset(bel, 0, sizeof bel);
		memset(is, 0, sizeof is);
		memset(dfn, 0, sizeof dfn);
		memset(low, 0, sizeof low);
		scanf("%d%d", &n, &m);
		if(!n && !m) exit(0);
		for(int i = 1; i <= m; ++i)
		{
			int u, v;
			scanf("%d%d", &u, &v);
			G.add_edge(u, v);
		}
	}
	void tarjan(int now, int fa)
	{
		dfn[now] = low[now] = ++time_f;
		for(int i = G.head[now]; i; i = G.ne[i])
			if(!dfn[G.to[i]])
			{
				tarjan(G.to[i], i);
				low[now] = min(low[now], low[G.to[i]]);
				if(low[G.to[i]] > dfn[now])
				{
					bridge[++count] = make_pair(now, G.to[i]);
					is[id(i)] = true;
				}
			}
			else if(id(i) != id(fa))
				low[now] = min(low[now], dfn[G.to[i]]);
	}
	inline void dfs(int now, int c)
	{
		bel[now] = c;
		for(int i = G.head[now]; i; i = G.ne[i])
			if(!bel[G.to[i]] && !is[id(i)])
				dfs(G.to[i], c);	
	}
	int fa[MAXN];
	bool vis[MAXN];
	int dep[MAXN];
	bool ok[MAXN];
	int F[MAXN];
	inline void get_fa()
	{
		queue<int> q;
		memset(vis, 0, sizeof vis);
		q.push(1), fa[1] = 1, vis[1] = true, dep[1] = 0;
		while(!q.empty())
		{
			int now = q.front();q.pop();
			for(int i = T.head[now]; i; i = T.ne[i])
				if(!vis[T.to[i]])
				{
					fa[T.to[i]] = now;
					vis[T.to[i]] = true;
					dep[T.to[i]] = dep[now]+1;
					q.push(T.to[i]);	
				}
		}
	}
	inline int find(int x)
	{
		int anc = x, y;
		while(F[anc] != anc)
			anc = F[anc];
		while(x != anc)
			y = F[x], F[x] = anc, x = y;
		return anc;	
	}
	inline void merge(int u, int v)
	{
		int fu = find(u), fv = find(v);
		if(fu != fv) F[fu] = fv;
	}
	inline void lca(int u, int v)
	{
		int s = 0;
		static int st[MAXN];
		u = find(u), v = find(v);
		st[++s] = u, st[++s] = v;
		while(dep[u] < dep[v])
		{
			if(ok[v])
			{
				ok[v] = false;
				count--;	
			}
			v = fa[v];
			v = find(v);
			st[++s] = v;
		}
		while(dep[u] > dep[v])
		{
			if(ok[u])
			{
				ok[u] = false;
				count--;	
			}
			u = fa[u];
			u = find(u);
			st[++s] = u;
		}
		while(u != v)
		{
			if(ok[v])
			{
				ok[v] = false;
				count--;	
			}
			v = fa[v];
			v = find(v);	
			st[++s] = v;
			if(ok[u])
			{
				ok[u] = false;
				count--;	
			}
			u = fa[u];
			u = find(u);
			st[++s] = u;
		}
		for(int i = 1; i <= s; ++i)
			merge(st[i], u);
	}
	inline void solve()
	{
		init();
		tarjan(1, 0);
		for(int i = 1; i <= n; ++i)
			if(!bel[i])
				dfs(i, ++tot);
		ok[1] = false;
		for(int i = 2; i <= tot; ++i)
			ok[i] = true;
		for(int i = 1; i <= tot; ++i)
			F[i] = i;
		for(int i = 1; i <= count; ++i)
			T.add_edge(bel[bridge[i].first], bel[bridge[i].second]);
		get_fa();
		int q;
		scanf("%d", &q);
		printf("Case %d:\n", ++Case);
		while(q--)
		{
			int u, v;
			scanf("%d%d", &u, &v);
			if(bel[u] != bel[v])
				lca(bel[u], bel[v]);
			printf("%d\n", count);
		}
		puts("");
	}
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("graph.in", "r", stdin);
	freopen("graph.out", "w", stdout);
#endif

	while(true)
		Graph::solve();

	return 0;
}
