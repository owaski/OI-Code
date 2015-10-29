#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cmath>
#define debug(...) fprintf(stderr, __VA_ARGS__)
#define OUT(x) ((x)<<1)
#define IN(x) (OUT(x)-1)
#define INV(x) (((x)-1)^1)+1
using namespace std;

const int R = 29;
const int N = (R*R)<<1;
const int M = N*N;
const int INF = 0x3f3f3f3f;

int r, c, d;
int g[R][R];
int S, T;
struct Net
{
	int sz;
	int head[N];
	int f[M];
	int to[M];
	int ne[M];
	Net()
	{
		sz = 1;
		memset(head, 0, sizeof head);
	}
	inline void add_edge(int u, int v, int flow)
	{
		to[sz] = v, f[sz] = flow, ne[sz] = head[u], head[u] = sz++;
		to[sz] = u, f[sz] = 0, ne[sz] = head[v], head[v] = sz++;
	}
}G;
int totL;
int dis[N];
int q[N], h, t;

inline int id(int x, int y)
{
	return (x-1)*c+y;
}

inline bool spfa()
{
	h = t = 0, q[++t] = S;
	memset(dis, INF, sizeof dis), dis[S] = 0;
	while(h < t)
	{
		int now = q[++h];
		for(int i = G.head[now]; i; i = G.ne[i])
			if(G.f[i] && dis[G.to[i]] > dis[now]+1)
			{
				dis[G.to[i]] = dis[now]+1;
				q[++t] = G.to[i];
			}
	}
	return dis[T] < INF;
}

inline int dfs(int now, int end, int flow)
{
	if(now == end || !flow) return flow;
	int tmp, ret = 0;
	for(int i = G.head[now]; i; i = G.ne[i])
		if(dis[G.to[i]] == dis[now]+1 && G.f[i] && (tmp = dfs(G.to[i], end, min(flow, G.f[i]))))
		{
			ret += tmp, flow -= tmp;
			G.f[i] -= tmp, G.f[INV(i)] += tmp;
			if(!flow) return ret;
		}
	dis[now] = INF;
	return ret;
}

inline int maxflow()
{
	int ret = 0;
	while(spfa())
		ret += dfs(S, T, INF);
	return ret;
}

int main()
{
	freopen("bzoj1066.in", "r", stdin);
	freopen("bzoj1066.out", "w", stdout);
	
	scanf("%d%d%d", &r, &c, &d);
	S = r*c*2+1, T = r*c*2+2;
	for(int i = 1; i <= r; ++i)
		for(int j = 1; j <= c; ++j)
		{
			scanf("%1d", &g[i][j]);
			int node = id(i, j);
			if(g[i][j])
			{
				G.add_edge(IN(node), OUT(node), g[i][j]);	
				if(i <= d || j <= d || r-i+1 <= d || c-j+1 <= d)
					G.add_edge(OUT(node), T, INF);
			}
		}
	for(int i = 1; i <= r; ++i)
		for(int j = 1; j <= c; ++j)
			if(g[i][j])
			{
				int u = id(i, j);
				for(int p = 1; p <= r; ++p)
					for(int q = 1; q <= c; ++q)
						if(abs(i-p)+abs(j-q) <= d && g[p][q] && !(p == i && q == j))
							G.add_edge(OUT(u), IN(id(p, q)), INF);
			}
	for(int i = 1; i <= r; ++i)
		for(int j = 1; j <= c; ++j)
		{
			char v;cin >> v;
			if(v == 'L') 
			{
				G.add_edge(S, IN(id(i, j)), 1);
				++totL;
			}
		}
	cout << totL-maxflow() << endl;
	
	fclose(stdin);fclose(stdout);
	return 0;
}
