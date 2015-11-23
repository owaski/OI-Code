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
	char ch;int f = 1;x = 0;
	while(((ch=getchar()) < '0' || ch > '9') && ch != '-');
	if(ch == '-') f = -1;else x = ch-'0';
	while((ch=getchar()) >= '0' && ch <= '9') x = x*10+ch-'0';
	x *= f;
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

const int N = 100009;
const int M = 100009;
const int Q = 100009;
const int K = 50;

int n, m, q;
long long val[M], w[N];
int c[N];

namespace Tree
{
	namespace Graph
	{
		int sz = 1;
		int head[N];
		int to[N<<1];
		int ne[N<<1];
		inline void addEdge(int u, int v)
		{
			to[sz] = v, ne[sz] = head[u], head[u] = sz++;
		}
	}
	
	int dis;
	int fa[N];
	int dep[N];
	int maxDis[N];
	int keyPot[K], keyN;
	int id[N];
	int cnt[M];
	int sum[N];
	int dfn[N<<1], dfnN;
	int pos[N][2];
	int q[K], sq[K];
	
	namespace Info
	{
		int sz = 1;
		int cnt[K*(K-1)/2][M];
		long long w[K][K];
		int lca[K][K];
	}
	
	namespace LCA
	{
		int f[N][20];
		void pre()
		{
			for(int i = 1; i <= n; ++i)
				f[i][0] = fa[i];
			for(int i = 1; i < 20; ++i)
				for(int j = 1; j <= n; ++j)
					f[j][i] = f[f[j][i-1]][i-1];
		}
	}
	
	void getKeyPot(int now, int fat)
	{
		maxDis[now] = 1;
		dfn[++dfnN] = now, pos[now][0] = dfnN;
		for(int i = Graph::head[now]; i; i = Graph::ne[i])
			if(Graph::to[i] != fat)
			{
				fa[Graph::to[i]] = now;
				dep[Graph::to[i]] = dep[now]+1;
				getKeyPot(Graph::to[i], now);
				maxDis[now] = max(maxDis[now], maxDis[Graph::to[i]]+1);
			}
		if(maxDis[now] > dis)
		{
			maxDis[now] = 0;
			keyPot[++keyN] = now;
			id[now] = keyN;
		}
		dfn[++dfnN] = now, pos[now][1] = dfnN;
	}
	
	inline int ID(int a, int b)
	{
		return keyN*(a-1)-((a*(a-1))>>1)+b-a;
	}
		
	void dfs(int now, int fat, int s, long long cost, int depth)
	{
		cost += val[c[now]]*w[++cnt[c[now]]];
		if(id[now])
		{
			Info::w[s][id[now]] = cost;
			if((++depth) > 1 && id[now] > s) 
				memcpy(Info::cnt[ID(s, id[now])]+1, cnt+1, m*sizeof(int));
		}
		for(int i = Graph::head[now]; i; i = Graph::ne[i])
			if(Graph::to[i] != fat)
				dfs(Graph::to[i], now, s, cost, depth);
		--cnt[c[now]];
	}
	
	inline int getLCA(int, int);
	
	void getInfo()
	{
		for(int i = 1; i <= keyN; ++i)
			dfs(keyPot[i], 0, i, 0, 0);
		for(int i = 1; i <= keyN; ++i)
			for(int j = 1; j <= keyN; ++j)
				Info::lca[i][j] = getLCA(keyPot[i], keyPot[j]);
	}
	
	void getSum(int now)
	{
		sum[now] += id[now] > 0;
		for(int i = Graph::head[now]; i; i = Graph::ne[i])
			if(fa[now] != Graph::to[i])
			{
				sum[Graph::to[i]] = sum[now];
				getSum(Graph::to[i]);
			}
	}
	
	inline void init()
	{
		dis = pow((double)n, (double)2/3);
		getKeyPot(1, 0), LCA::pre(), getInfo(), getSum(1);
	}
	
	inline void up(int &x, int y)
	{
		int i = 0;
		while(y)
		{
			if(y&1) x = LCA::f[x][i];
			i++, y >>= 1;
		}
	}
	
	inline int getLCA(int u, int v)
	{
		while(dep[u] > dep[v]) up(u, dep[u]-dep[v]);
		while(dep[v] > dep[u]) up(v, dep[v]-dep[u]);
		while(u != v)
		{
			for(int i = 19; i >= 0; --i)
				if(LCA::f[u][i] != LCA::f[v][i])
					u = LCA::f[u][i], v = LCA::f[v][i];
			if(LCA::f[u][0] == LCA::f[v][0])
				return LCA::f[u][0];
		}
		return u;
	}
	
	inline int getUp(int x)
	{
		while(true)
		{
			for(int i = 19; i >= 0; --i)
				if(sum[x] == sum[LCA::f[x][i]])
					x = LCA::f[x][i];
			if(sum[x] == sum[fa[x]]+1)
				return x;
		}
	}
	
	inline int getUp(int x, int y)
	{
		while(true)
		{
			for(int i = 19; i >= 0; --i)
				if(sum[LCA::f[x][i]]-sum[y] >= 1)
					x = LCA::f[x][i];
			if(sum[fa[x]] == sum[y])
				return x;
		}
	}
	
	inline pair<int,int> get(int u, int v)
	{
		int lca = getLCA(u, v);
		if(sum[u]-sum[fa[lca]]+sum[v]-sum[lca] <= 1)
			return make_pair(lca, lca);
		if(sum[u]-sum[lca] > 0) u = getUp(u);
		else u = getUp(v, fa[lca]);
		if(sum[v]-sum[lca] > 0) v = getUp(v);
		else v = getUp(u, fa[lca]);
		return make_pair(u, v);
	}
	
	inline int get(int u, int v, int C)
	{
		if(u == v) return 0;
		if(u > v) swap(u, v);
		return Info::cnt[ID(u, v)][C];
	}
	
	int used[N], tot;
	
	inline void calc(long long &ret, int u, int t, pair<int,int> &p)
	{
		for(int i = u; i != t; i = fa[i])
		{
			if(cnt[c[i]])
				ret += val[c[i]]*w[++cnt[c[i]]];
			else ret += val[c[i]]*w[cnt[c[i]] = get(id[p.first], id[p.second], c[i])+1];
			used[++tot] = c[i];
		}
	}
	
	inline void clr()
	{
		for(int i = 1; i <= tot; ++i)
			cnt[used[i]] = 0;
		tot = 0;
	}
	
	inline bool isAnc(int x, int y)
	{
		return pos[x][0] <= pos[y][0] && pos[y][1] <= pos[x][1];
	}
	
	inline bool onRoad(int x, int u, int v)
	{
		return isAnc(Info::lca[id[u]][id[v]], x) && (isAnc(x, u) || isAnc(x, v));
	}
	
	inline long long query(int u, int v)
	{
		int lca = getLCA(u, v);
		pair<int,int> p = get(u, v);
		long long ret = 0;
		if(p.first == p.second) ret = 0;
		else if(id[p.first] < id[p.second]) ret = Info::w[id[p.first]][id[p.second]];
		else ret = Info::w[id[p.second]][id[p.first]];
		if(isAnc(p.first, u)) calc(ret, u, p.first, p);
		else calc(ret, u, lca, p), calc(ret, fa[p.first], fa[lca], p);
		if(isAnc(p.second, v)) calc(ret, v, p.second, p);
		else calc(ret, v, lca, p), calc(ret, fa[p.second], fa[lca], p);
		if(p.first == p.second)
		{
			ret += val[c[p.first]]*w[++cnt[c[p.first]]];
			used[++tot] = c[p.first];
		}
		clr();
		return ret;
	}
	
	inline void modify(int x, int y)
	{
		for(int i = 1; i <= keyN; ++i)
			for(int j = i+1; j <= keyN; ++j)
				if(onRoad(x, keyPot[i], keyPot[j]))
				{
					Info::w[i][j] -= val[c[x]]*w[Info::cnt[ID(i, j)][c[x]]--];
					Info::w[i][j] += val[y]*w[++Info::cnt[ID(i, j)][y]];
				}
		c[x] = y;
	}
}

inline void init()
{
	read(n), read(m), read(q);
	for(int i = 1; i <= m; ++i)
		read(val[i]);
	for(int i = 1; i <= n; ++i)
		read(w[i]);
	for(int i = 1; i < n; ++i)
	{
		int u, v;
		read(u), read(v);
		Tree::Graph::addEdge(u, v);
		Tree::Graph::addEdge(v, u);
	}
	for(int i = 1; i <= n; ++i)
		read(c[i]);
	Tree::init();
}

inline void solve()
{
	while(q--)
	{
		int t, x, y;
		read(t), read(x), read(y);
		if(t) write(Tree::query(x, y)), putchar('\n');
		else Tree::modify(x, y);
	}
	debug("%lfs\n", (double)clock()/CLOCKS_PER_SEC);
}

int main()
{
	freopen("park.in", "r", stdin);
	freopen("park.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
