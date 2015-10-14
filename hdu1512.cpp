#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

const int N = 100009;

int n, m;
int key[N];
namespace Union
{
	int fa[N];
	int rk[N];
	inline void init()
	{
		for(int i = 1; i <= n; ++i)
			fa[i] = i, rk[i] = 1;
	}
	inline int find(int x)
	{
		int anc = x, y;
		while(anc != fa[anc])
			anc = fa[anc];
		while(x != anc)
			y = fa[x], fa[x] = anc, x = y;
		return anc;
	}
	inline bool same(int u, int v)
	{
		return find(u) == find(v);
	}
	inline void merge(int u, int v)
	{
		int p = find(u), q = find(v);
		if(rk[p] > rk[q]) swap(p, q);
		fa[p] = q, rk[q] = max(rk[q], rk[p]+1);
	}
}
namespace Leftist_tree
{
	int fa[N];
	int c[N][2];
	int dis[N];
	inline void init()
	{
		dis[0] = -1;
		for(int i = 1; i <= n; ++i)
			fa[i] = c[i][0] = c[i][1] = dis[i] = 0;
	}
	int merge(int u, int v)
	{
		if(!u) return v;
		if(!v) return u;
		if(key[u] < key[v]) swap(u, v);
		c[u][1] = merge(v, c[u][1]);
		fa[c[u][1]] = u;
		if(dis[c[u][0]] < dis[c[u][1]])
			swap(c[u][0], c[u][1]);
		dis[u] = dis[c[u][1]]+1;
		return u;
	}
	inline int find(int u)
	{
		while(fa[u]) u = fa[u];
		return u;
	}
	inline int modify(int u)
	{
		int p = merge(c[u][0], c[u][1]);
		fa[p] = 0;key[u] >>= 1;
		fa[u] = c[u][0] = c[u][1] = dis[u] = 0;
		return merge(p, u);
	}
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("hdu1512.in", "r", stdin);
	freopen("hdu1512.out", "w", stdout);
#endif

	while(cin >> n)
	{
		Union::init();
		Leftist_tree::init();
		for(int i = 1; i <= n; ++i)
			scanf("%d", key+i);
		cin >> m;
		while(m--)
		{
			int u, v;
			scanf("%d%d", &u, &v);
			if(Union::same(u, v)) 
				puts("-1");
			else
			{
				u = Union::find(u);
				v = Union::find(v);
				Union::merge(u, v);
				u = Leftist_tree::find(u);
				v = Leftist_tree::find(v);
				u = Leftist_tree::modify(u);
				v = Leftist_tree::modify(v);
				printf("%d\n", key[Leftist_tree::merge(u, v)]);
			}
		}
	}

	return 0;
}
