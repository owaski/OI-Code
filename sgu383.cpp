#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int MAXN = 100009;

namespace Point
{
	struct Pot
	{
		int id;
		double x, y;
		Pot() {}
		Pot(int _x, int _y) {x = _x, y = _y;}
		inline void input()
		{
			scanf("%lf%lf", &x, &y);	
		}
		inline friend Pot operator + (const Pot &a, const Pot &b)
		{
			return Pot(a.x+b.x, a.y+b.y);	
		}
		inline friend Pot operator - (const Pot &a, const Pot &b)
		{
			return Pot(a.x-b.x, a.y-b.y);
		}
		inline friend double operator * (const Pot &a, const Pot &b)
		{
			return a.x*b.x+a.y*b.y;
		}	
		inline friend double operator ^ (const Pot &a, const Pot &b)
		{
			return a.x*b.y-a.y*b.x;	
		}
		inline friend Pot operator * (const Pot &a, const double &k)
		{
			return Pot(a.x*k, a.y*k);	
		}
		inline friend Pot operator / (const Pot &a, const double &k)
		{
			return Pot(a.x/k, a.y/k);	
		}
		inline friend bool operator < (const Pot &a, const Pot &b)
		{
			return a.x == b.x ? a.y < b.y : a.x < b.x;	
		}
	};
	
	struct Pot3D
	{
		double x, y, z;
		Pot3D() {}
		Pot3D(double _x, double _y, double _z) {x = _x, y = _y, z = _z;}
		Pot3D(const Pot &a) {x = a.x, y = a.y, z = x*x+y*y;}
		inline friend Pot3D operator + (const Pot3D &a, const Pot3D &b)
		{
			return Pot3D(a.x+b.x, a.y+b.y, a.z+b.z);	
		}
		inline friend Pot3D operator - (const Pot3D &a, const Pot3D &b)
		{
			return Pot3D(a.x-b.x, a.y-b.y, a.z-b.z);	
		}
		inline friend double operator * (const Pot3D &a, const Pot3D &b)
		{
			return a.x*b.x+a.y*b.y+a.z*b.z;
		}	
		inline friend Pot3D operator ^ (const Pot3D &a, const Pot3D &b)
		{
			return	Pot3D(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);
		}
		inline friend Pot3D operator * (const Pot3D &a, const double &k)
		{
			return Pot3D(a.x*k, a.y*k, a.z*k);	
		}
		inline friend Pot3D operator / (const Pot3D &a, const double &k)
		{
			return Pot3D(a.x/k, a.y/k, a.z/k);	
		}
	};
	
	#define sqr(x) ((x)*(x))
	inline double dis(const Pot &a, const Pot &b)
	{
		return sqrt(sqr(a.x-b.x)+sqr(a.y-b.y));	
	}
	inline double cross(const Pot &a, const Pot &b, const Pot &c)
	{
		return (b-a)^(c-a);	
	}
	inline bool inter(const Pot &a, const Pot &b, const Pot &c, const Pot &d)
	{
		return ((c-a)^(b-a))*((d-a)^(b-a)) < 0 && ((a-c)^(d-c))*((b-c)^(d-c)) < 0;
	}	
	inline bool in_cir(const Pot &a, const Pot &b, const Pot &c, const Pot &d)
	{
		Pot3D ta = Pot3D(a), tb = Pot3D(b), tc = Pot3D(c), td = Pot3D(d);
		Pot3D law = (tb-ta)^(tc-ta);
		return law*(td-ta) < 0;
	}
}

struct Tree
{
	#define inv(x) (((x-1)^1)+1)
	int sz;
	int head[MAXN];
	int to[MAXN<<5];
	double w[MAXN<<5];
	int ne[MAXN<<5];
	int pr[MAXN<<5];
	Tree()
	{
		sz = 1;
		memset(head, 0, sizeof head);
		memset(pr, 0, sizeof pr);
		memset(ne, 0, sizeof ne);
	}
	inline void add_edge(int u, int v, double d)
	{
		to[sz] = v, w[sz] = d, ne[sz] = head[u], pr[head[u]] = sz, head[u] = sz++;
		to[sz] = u, w[sz] = d, ne[sz] = head[v], pr[head[v]] = sz, head[v] = sz++;	
	}
	inline void del_edge(int i)
	{
		pr[ne[i]] = pr[i], ne[pr[i]] = ne[i];
	}
};

struct Edge
{
	int u, v;
	double w;	
	inline friend bool operator < (const Edge &a, const Edge &b)
	{
		return a.w < b.w;	
	}
};

namespace Delaunay_triangulation
{
	using namespace Point;
	int n, q;
	Pot p[MAXN];
	int mp[MAXN];
	int st[MAXN], top;
	Tree G, H;
	Edge e[MAXN<<5];
	int tot;
	inline void init()
	{
		scanf("%d", &n);
		for(int i = 1; i <= n; ++i)
		{
			p[i].input();
			p[i].id = i;
		}
	}
	void divide_and_conquer(int l, int r)
	{
		if(r-l+1 <= 3)
		{
			for(int i = l; i <= r; ++i)
				for(int j = i+1; j <= r; ++j)
					G.add_edge(i, j, dis(p[i], p[j]));
			return ;
		}
		int mid = (l+r)>>1, L, R;
		divide_and_conquer(l, mid);
		divide_and_conquer(mid+1, r);
		top = 0;
		for(int i = l; i <= r; ++i)
		{
			while(top >= 2 && cross(p[st[top-1]], p[st[top]], p[i]) < 0)
				top--;
			st[++top] = i;	
		}
		for(int i = 1; i < top; ++i)
			if(st[i] <= mid && st[i+1] > mid)
			{
				L = st[i], R = st[i+1];
				break;	
			}
		G.add_edge(L, R, dis(p[L], p[R]));
		while(true)
		{
			int cur = 0, ch = 0;
			for(int i = G.head[L]; i; i = G.ne[i])
				if(cross(p[L], p[R], p[G.to[i]]) > 0 && (!cur || in_cir(p[L], p[R], p[cur], p[G.to[i]]))) cur = G.to[i], ch = -1;
			for(int i = G.head[R]; i; i = G.ne[i])
				if(cross(p[R], p[L], p[G.to[i]]) < 0 && (!cur || in_cir(p[L], p[R], p[cur], p[G.to[i]]))) cur = G.to[i], ch = 1;
			if(!cur) break;
			if(ch == -1)
			{
				for(int i = G.head[L]; i; i = G.ne[i])
					if(inter(p[L], p[G.to[i]], p[R], p[cur]))
					{
						G.del_edge(i);
						G.del_edge(inv(i));	
					}
				L = cur;
			}
			else
			{
				for(int i = G.head[R]; i; i = G.ne[i])
					if(inter(p[R], p[G.to[i]], p[L], p[cur]))
					{
						G.del_edge(i);
						G.del_edge(inv(i));	
					}
				R = cur;	
			}
			G.add_edge(L, R, dis(p[L], p[R]));
		}
	}
	
	int fa[MAXN];
	int rk[MAXN];
	inline int find(int x)
	{
		int anc = x, y;
		while(anc != fa[anc])
			anc = fa[anc];
		while(x != anc)
			y = fa[x], fa[x] = anc, x = y;
		return anc;
	}
	inline void merge(int x, int y, double w)
	{
		int fx = find(x), fy = find(y);
		if(fx == fy) return ;
		if(rk[fx] > rk[fy]) swap(fx, fy);
		fa[fx] = fy, rk[fy] = max(rk[fx]+1, rk[fy]);
		H.add_edge(x, y, w);
	}
	inline void mst()
	{
		for(int i = 1; i <= n; ++i)
			fa[i] = i, rk[i] = 1;
		sort(e+1, e+tot+1);
		for(int i = 1; i <= tot; ++i)
			merge(e[i].u, e[i].v, e[i].w);	
	}
	int dep[MAXN];
	double d[MAXN][20];
	int f[MAXN][20];
	bool vis[MAXN];
	inline void pre_do()
	{
		queue<int> q;
		q.push(1), vis[1] = true;
		while(!q.empty())
		{
			int now = q.front();q.pop();
			for(int i = H.head[now]; i; i = H.ne[i])
				if(!vis[H.to[i]])
				{
					vis[H.to[i]] = true;
					f[H.to[i]][0] = now;
					d[H.to[i]][0] = H.w[i];
					dep[H.to[i]] = dep[now]+1;
					q.push(H.to[i]);
				}
		}
		for(int i = 1; i < 20; ++i)
			for(int j = 1; j <= n; ++j)
			{
				f[j][i] = f[f[j][i-1]][i-1];
				d[j][i] = max(d[j][i-1], d[f[j][i-1]][i-1]);
			}
	}
	inline double up(int &x, int y)
	{
		int i = 0;
		double ret = 0;
		while(y)
		{
			if(y&1) 
			{
				ret = max(ret, d[x][i]);
				x = f[x][i];
			}
			y >>= 1;
			i++;
		}
		return ret;
	}
	inline double qry(int u, int v)
	{
		double ret = 0;
		if(dep[u] > dep[v]) ret = max(ret, up(u, dep[u]-dep[v]));
		if(dep[v] > dep[u]) ret = max(ret, up(v, dep[v]-dep[u]));
		while(u != v)
		{
			for(int i = 19; i >= 0; --i)
				if(f[u][i] != f[v][i])
				{
					ret = max(ret, max(d[u][i], d[v][i]));
					u = f[u][i], v = f[v][i];
				}
			if(f[u][0] == f[v][0])
			{
				ret = max(ret, max(d[u][0], d[v][0]));
				u = f[u][0], v = f[v][0];	
			}
		}
		return ret;
	}
	inline void solve()
	{
		init();
		sort(p+1, p+n+1);
		for(int i = 1; i <= n; ++i)
			mp[p[i].id] = i;
		divide_and_conquer(1, n);
		for(int i = 1; i <= n; ++i)
			for(int j = G.head[i]; j; j = G.ne[j])
				if(j&1)
				{
					++tot;
					e[tot].u = i;
					e[tot].v = G.to[j];
					e[tot].w = G.w[j];	
				}
		mst();
		pre_do();
		int m;
		scanf("%d", &m);
		while(m--)
		{
			int x, y;
			scanf("%d%d", &x, &y);
			printf("%.10lf\n", qry(mp[x], mp[y]));	
		}
	}
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("sgu383.in", "r", stdin);
	freopen("sgu383.out", "w", stdout);
#endif

	Delaunay_triangulation::solve();

	return 0;
}
