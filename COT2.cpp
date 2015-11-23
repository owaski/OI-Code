#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

template<class T>
inline void read(T &x)
{
	char c;int t = 1;x = 0;
	while(((c=getchar()) < '0' || c > '9') && c != '-');
	if(c == '-') t = -1;else x = c-'0';
	while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
	x *= t;
}

int OutN;
char Out[20];

template<class T>
inline void write(T x)
{
	if(x < 0) x = -x, putchar('-');
	if(x)
	{
		OutN = 0;
		while(x)
		{
			Out[OutN++] = x%10+'0';
			x /= 10;
		}
		while(OutN--)
			putchar(Out[OutN]);
	}
	else putchar('0');
}

const int N = 40009;
const int M = 100009;
const int K = 209;

int n, m;
int w[N], val[N], valN;
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
	inline void addEdge(int u, int v)
	{
		to[sz] = v, ne[sz] = head[u], head[u] = sz++;
	}
}G;

int S;
int key[N], keyN;
int idKey[N];
int maxDep[N];
int sum[N];

int depth[N];
int f[N][17];

int cnt[N];
int used[N], tot;

struct Query
{
	int sz;
	int head[K];
	int v[M];
	int u[M];
	int id[M];
	int ne[M];
	Query()
	{
		sz = 1;
		memset(head, 0, sizeof head);
	}
	inline void add(int k, int _u, int _v, int _id)
	{
		u[sz] = _u, v[sz] = _v, id[sz] = _id, ne[sz] = head[k], head[k] = sz++;
	}
}Q;

struct Qeury2
{
	int sz;
	int head[N];
	int u[M];
	int id[M];
	int ne[M];
	inline void init()
	{
		sz = 1;
		memset(head, 0, sizeof head);
	}
	inline void add(int _u, int _v, int _id)
	{
		u[sz] = _u, id[sz] = _id, ne[sz] = head[_v], head[_v] = sz++;
	}
}Q2;

int ans[M];

inline void init()
{
	read(n), read(m);
	for(int i = 1; i <= n; ++i)
		read(w[i]), val[++valN] = w[i];
	sort(val+1, val+valN+1);
	valN = unique(val+1, val+valN+1)-val-1;
	for(int i = 1; i <= n; ++i)
		w[i] = lower_bound(val+1, val+valN+1, w[i])-val;
	for(int i = 1; i < n; ++i)
	{
		int u, v;
		read(u), read(v);
		G.addEdge(u, v);
		G.addEdge(v, u);
	}
}

void getKey(int now, int fa)
{
	maxDep[now] = 1;
	for(int i = G.head[now]; i; i = G.ne[i])
		if(G.to[i] != fa)
		{
			depth[G.to[i]] = depth[now]+1;
			f[G.to[i]][0] = now, getKey(G.to[i], now);
			maxDep[now] = max(maxDep[now], maxDep[G.to[i]]+1);
		}
	if(maxDep[now] > S)
	{
		key[++keyN] = now;
		idKey[now] = keyN;
		maxDep[now] = 0;
	}
}

void getSum(int now, int fa)
{
	sum[now] += idKey[now] > 0;
	for(int i = G.head[now]; i; i = G.ne[i])
		if(G.to[i] != fa)
		{
			sum[G.to[i]] = sum[now];
			getSum(G.to[i], now);
		}
}

inline void preLCA()
{
	for(int i = 1; i <= 16; ++i)
		for(int j = 1; j <= n; ++j)
			f[j][i] = f[f[j][i-1]][i-1];
}

inline void up(int &x, int y)
{
	int i = 0;
	while(y)
	{
		if(y&1) x = f[x][i];
		i++, y >>= 1;
	}
}

inline int LCA(int u, int v)
{
	if(depth[u] > depth[v]) up(u, depth[u]-depth[v]);
	if(depth[u] < depth[v]) up(v, depth[v]-depth[u]);
	while(u != v)
	{
		for(int i = 16; i >= 0; --i)
			if(f[u][i] != f[v][i])
				u = f[u][i], v = f[v][i];
		if(f[u][0] == f[v][0])
			u = f[u][0], v = f[v][0];
	}
	return u;
}

void clr()
{
	for(int i = 1; i <= tot; ++i)
		cnt[used[i]] = 0;
	tot = 0;
}

int getUp(int x)
{
	while(!idKey[x])
	{
		for(int i = 16; i >= 0; --i)
			if(sum[f[x][i]] == sum[x])
				x = f[x][i];
	}
	return idKey[x];
}

inline void work(int u, int v, int id)
{
	int lca = LCA(u, v);
	if(sum[u]-sum[lca]+sum[v]-sum[f[lca][0]] == 0)
	{
		for(int i = u; i != lca; i = f[i][0])
			ans[id] += (++cnt[w[i]]) == 1, used[++tot] = w[i];
		for(int i = v; i != f[lca][0]; i = f[i][0])
			ans[id] += (++cnt[w[i]]) == 1, used[++tot] = w[i];
		clr();return ;
	}
	if(sum[u]-sum[f[lca][0]] == 0) swap(u, v); 
	Q.add(getUp(u), u, v, id);
}

int curAns;
int fa[N];

void getFa(int now, int fat)
{
	for(int i = G.head[now]; i; i = G.ne[i])
		if(G.to[i] != fat)
		{
			fa[G.to[i]] = now;
			getFa(G.to[i], now);
		}
}

void getAns(int u, int id)
{
	int x = u;
	while(fa[x])
	{
		curAns += (++cnt[w[x]]) == 1;
		x = fa[x];
	}
	ans[id] = curAns;
	while(fa[u])
	{
		curAns -= (cnt[w[u]]--) == 1;
		u = fa[u];
	}
}

inline void work(int now)
{
	curAns += (++cnt[w[now]]) == 1;
	for(int i = Q2.head[now]; i; i = Q2.ne[i])
		getAns(Q2.u[i], Q2.id[i]);
	for(int i = G.head[now]; i; i = G.ne[i])
		if(G.to[i] != fa[now])
			work(G.to[i]);
	curAns -= (cnt[w[now]]--) == 1;
}

void solve()
{
	S = sqrt(n);
	getKey(1, 0), getSum(1, 0), preLCA();
	for(int i = 1; i <= m; ++i)
	{
		int u, v;
		read(u), read(v);
		work(u, v, i);
	}
	for(int i = 1; i <= keyN; ++i)
	{
		Q2.init();
		memset(fa, 0, sizeof fa);
		for(int j = Q.head[i]; j; j = Q.ne[j])
			Q2.add(Q.u[j], Q.v[j], Q.id[j]);
		getFa(key[i], 0);
		work(key[i]);
	}
	for(int i = 1; i <= m; ++i)
		write(ans[i]), putchar('\n');
}

int main()
{
	freopen("COT2.in", "r", stdin);
	freopen("COT2.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
