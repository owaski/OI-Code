#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 509;
const int M = 5009;
const int INF = 1e9;

int n, m;

namespace Graph
{
	struct Edge
	{
		int u, v, d;
		inline void input()
		{
			scanf("%d%d%d", &u, &v, &d);
		}
		inline friend bool operator < (const Edge &a, const Edge &b)
		{
			return a.d < b.d;
		}
	}e[M];
	struct Ugraph
	{
		int sz;
		int head[N];
		int to[M<<1];
		int d[M<<1];
		int ne[M<<1];
		Ugraph()
		{
			sz = 1;
			memset(head, 0, sizeof head);	
		}
		inline void add_edge(int u, int v, int _d)
		{
			to[sz] = v, d[sz] = _d, ne[sz] = head[u], head[u] = sz++;	
			to[sz] = u, d[sz] = _d, ne[sz] = head[v], head[v] = sz++;
		}
	}G;
	
	int s, t;
	int f[N];
	int ax = INF, ay = 1;
	inline void input()
	{
		for(int i = 1; i <= m; ++i)
			e[i].input();
		scanf("%d%d", &s, &t);
	}
	inline void spfa(int s)
	{
		queue<int> q;
		static bool vis[N];
		q.push(s), vis[s] = true;
		while(!q.empty())
		{
			int now = q.front();
			vis[now] = false, q.pop();
			for(int i = G.head[now]; i; i = G.ne[i])
				if(f[G.to[i]] > max(f[now], G.d[i]))
				{
					f[G.to[i]] = max(f[now], G.d[i]);
					if(!vis[G.to[i]])
					{
						vis[G.to[i]] = true;
						q.push(G.to[i]);
					}
				}
		}
	}
	inline void solve()
	{
		for(int i = 1; i <= n; ++i)
			f[i] = INF;
		f[s] = 0;sort(e+1, e+m+1);
		for(int i = m; i >= 1; --i)
		{
			G.add_edge(e[i].u, e[i].v, e[i].d);
			if(f[e[i].u] > f[e[i].v]) swap(e[i].u, e[i].v);
			if(max(f[e[i].u], e[i].d) >= f[e[i].v]) continue;
			if(f[e[i].u] == INF) continue;
			spfa(e[i].u);
			if((long long)f[t]*ay < (long long)ax*e[i].d)
				ax = f[t], ay = e[i].d;
		}
	}
	inline void output()
	{
		if(ax == INF) puts("IMPOSSIBLE");
		else
		{
			int d = __gcd(ax, ay);
			ax /= d, ay /= d;
			if(ay == 1) printf("%d\n", ax);
			else printf("%d/%d\n", ax, ay);
		}
	}
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1050.in", "r", stdin);
	freopen("spfa.out", "w", stdout);
#endif

	scanf("%d%d", &n, &m);
	using namespace Graph;
	input(), solve(), output();

	return 0;
}
