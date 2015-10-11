#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 509;
const int M = 5009;
const int INF = 30000;

int n, m;
struct Edge
{
	int u, v, w;
	inline void input()
	{
		scanf("%d%d%d", &u, &v, &w);	
	}
	inline friend bool operator < (const Edge &a, const Edge &b)
	{
		return a.w < b.w;
	}
}e[M];

int s, t;

int fa[N];
int rk[N];

struct Tree
{
	int sz;
	int head[N];
	int to[N<<1];
	int d[N<<1];
	int ne[N<<1];
	inline void init()
	{
		sz = 1;
		memset(head, 0, sizeof head);
	}
	inline void add_edge(int u, int v, int _d)
	{
		to[sz] = v, d[sz] = _d, ne[sz] = head[u], head[u] = sz++;
		to[sz] = u, d[sz] = _d, ne[sz] = head[v], head[v] = sz++;	
	}
}T;

inline int gcd(int a, int b)
{
	return b?gcd(b, a%b):a;	
}

struct Frac
{
	int x, y;
	Frac() {x = 0, y = 1;}
	Frac(int _x, int _y):x(_x), y(_y) {int t = gcd(x, y);x /= t, y /= t;}
	inline friend bool operator < (const Frac &a, const Frac &b)
	{
		return a.x*b.y < b.x*a.y;	
	}
	inline void output()
	{
		if(y == 1) printf("%d\n", x);
		else printf("%d/%d\n", x, y);	
	}
}ans;

inline void init()
{
	T.init();
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

inline void merge(int u, int v, int d)
{
	int fu = find(u), fv = find(v);
	if(fu != fv)
	{
		if(rk[fu] > rk[fv]) swap(fu, fv);
		fa[fu] = fv, rk[fv] = max(rk[fv], rk[fu]+1);
		T.add_edge(u, v, d);
	}
}

inline int calc(int s, int t)
{
	queue<int> q, v;
	static bool vis[N];
	memset(vis, false, sizeof vis);
	q.push(s), v.push(0), vis[s] = true;
	while(!q.empty())
	{
		int now = q.front();q.pop();
		int val = v.front();v.pop();
		if(now == t) return val;
		for(int i = T.head[now]; i; i = T.ne[i])
			if(!vis[T.to[i]])
			{
				vis[T.to[i]] = true;
				q.push(T.to[i]);
				v.push(max(val, T.d[i]));	
			}
	}
	return INF;
}

inline void solve(int mine)
{
	if((s == e[mine].u && t == e[mine].v) || (s == e[mine].v && t == e[mine].u))
	{
		ans = Frac(1, 1);
		return ;	
	}
	for(int i = mine; i <= m; ++i)
		merge(e[i].u, e[i].v, e[i].w);
	int up = max(calc(s, e[mine].u), calc(e[mine].v, t));
	up = min(up, max(calc(s, e[mine].v), calc(e[mine].u, t)));
	if(up < INF) ans = min(ans, Frac(up, e[mine].w));
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1050.in", "r", stdin);
	freopen("bzoj1050.out", "w", stdout);
#endif

	scanf("%d%d", &n, &m);
	for(int i = 1; i <= m; ++i)
		e[i].input();
	scanf("%d%d", &s, &t);
	sort(e+1, e+m+1);
	ans = Frac(INF, 1);
	for(int i = 1; i <= m; ++i)
	{
		init();
		solve(i);
	}
	if(ans.x < INF) ans.output();
	else puts("IMPOSSIBLE");

	return 0;
}
