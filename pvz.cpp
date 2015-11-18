#include <cstdio>
#include <vector>
#include <cstring>
#include <iostream>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

template<class T>
inline void read(T &x)
{
	char c;int f = 1;x = 0;
	while(((c=getchar()) < '0' || c > '9') && c != '-');
	if(c == '-') f = -1;else x = c-'0';
	while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
	x *= f;
}

const int N = 29;
const int M = 39;
const int NODE = N*M;
const int EDGE = NODE*NODE;
const int INF = 1e9;

int n, m;
struct Net
{
	int sz;
	int head[NODE];
	int to[EDGE];
	int f[EDGE];
	int ne[EDGE];
	Net()
	{
		sz = 1;
		memset(head, 0, sizeof head);
	}
	int inv(int x)
	{
		return ((x-1)^1)+1;
	}
	void addEdge(int u, int v, int flow)
	{
		to[sz] = v, f[sz] = flow, ne[sz] = head[u], head[u] = sz++;
		to[sz] = u, f[sz] = 0, ne[sz] = head[v], head[v] = sz++;
	}
}G;
int ind[NODE];
vector<int> e[NODE];
int S, T;
int posSum;
int q[NODE];
bool vis[NODE];
int dis[NODE];
int score[NODE];

int id(int x, int y)
{
	return (x-1)*m+y;
}

void init()
{
	read(n), read(m);
	S = n*m+1, T = S+1;
	for(int i = 1; i <= n; ++i)
		for(int j = 1; j <= m; ++j)
		{
			int curId = id(i, j);read(score[curId]);
			if(j < m) e[id(i, j+1)].push_back(curId), ind[curId]++;
			int attackSum = 0;read(attackSum);
			while(attackSum--)
			{
				int x, y;
				read(x), read(y), x++, y++;
				e[curId].push_back(id(x, y));
				ind[id(x, y)]++;
			}
		}
}

void topSort()
{
	int head = 0, tail = 0;
	for(int i = 1; i <= n*m; ++i)
		if(ind[i] == 0)
		{
			q[++tail] = i;
			vis[i] = true;
		}
	while(head < tail)
	{
		int now = q[++head];
		for(int i = 0; i < (int)e[now].size(); ++i)
			if((--ind[e[now][i]]) == 0)
			{
				q[++tail] = e[now][i];
				vis[e[now][i]] = true;
			}
	}
	vis[S] = vis[T] = true;
}

void constructNet()
{
	for(int i = 1; i <= n*m; ++i)
		if(vis[i])
		{
			if(score[i] >= 0) G.addEdge(S, i, score[i]), posSum += score[i];
			else G.addEdge(i, T, -score[i]);
			for(int j = 0; j < (int)e[i].size(); ++j)
				if(vis[e[i][j]])
					G.addEdge(e[i][j], i, INF);
		}
}

bool spfa()
{
	int head = 0, tail = 0;
	for(int i = 1; i <= T; ++i) dis[i] = INF;
	q[++tail] = S, dis[S] = 0;
	while(head < tail)
	{
		int now = q[++head];
		for(int i = G.head[now]; i; i = G.ne[i])
			if(vis[G.to[i]] && G.f[i] && dis[now]+1 < dis[G.to[i]])
			{
				dis[G.to[i]] = dis[now]+1;
				q[++tail] = G.to[i];
			}
	}
	return dis[T] < INF;
}

int dfs(int now, int end, int flow)
{
	if(now == end || !flow) return flow;
	int ret = 0, tmp;
	for(int i = G.head[now]; i; i = G.ne[i])
		if(vis[G.to[i]] && dis[G.to[i]] == dis[now]+1 && G.f[i] && (tmp = dfs(G.to[i], end, min(flow, G.f[i]))))
		{
			ret += tmp, flow -= tmp;
			G.f[i] -= tmp, G.f[G.inv(i)] += tmp;
			if(!flow) return ret;
		}
	dis[now] = INF;
	return ret;
}

int netFlow()
{
	int ret = 0;
	while(spfa())
		ret += dfs(S, T, INF);
	return ret;
}

void solve()
{
	topSort();
	constructNet();
	int maxFlow = netFlow();
	cout << posSum-maxFlow << endl;
}

int main()
{
	freopen("pvz.in", "r", stdin);
	freopen("pvz.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
