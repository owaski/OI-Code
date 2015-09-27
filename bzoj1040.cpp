#include <bits/stdc++.h>
using namespace std;

template<class T> inline void read(T &x)
{
	char c;x = 0;
	while((c=getchar()) < '0' && c > '9');
	x = c-'0';
	while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';	
}

const int MAXN = 1000009;
const long long INF = 1LL<<60;

struct Ugraph
{
	int sz;
	int head[MAXN];
	int to[MAXN<<1];
	int ne[MAXN<<1];
	Ugraph()
	{
		sz = 1;
		memset(head, 0, sizeof head);
	}
	inline void add_edge(int u, int v)
	{
		to[sz] = v, ne[sz] = head[u], head[u] = sz++;
		to[sz] = u, ne[sz] = head[v], head[v] = sz++;	
	}
}G;

int n;
int w[MAXN];
bool vis[MAXN];
int dfn[MAXN];
int low[MAXN], tot;
int fa[MAXN];
long long f[MAXN][2];
long long ans;

void dfs(int now)
{
	dfn[now] = low[now] = ++tot;
	for(int i = G.head[now]; i; i = G.ne[i])
		if(!dfn[G.to[i]])
		{
			fa[G.to[i]] = now, dfs(G.to[i]);
			low[now] = min(low[now], low[G.to[i]]);	
		}
		else if(G.to[i] != fa[now])
			low[now] = min(low[now], dfn[G.to[i]]);
	f[now][1] = w[now];set<int> s;
	for(int i = G.head[now]; i; i = G.ne[i])
		if(fa[G.to[i]] == now && low[G.to[i]] > dfn[now])
			s.insert(G.to[i]);
		else if(fa[G.to[i]] != now && dfn[G.to[i]] > dfn[now])
		{
			vector<int> ring;
			int cur = G.to[i];
			ring.push_back(cur);
			while(cur != now)
			{
				cur = fa[cur];
				ring.push_back(cur);	
			}
			long long l0 = f[ring[0]][0], l1 = -INF;
			long long ans0 = 0, ans1 = 0;
			for(int i = 1; i < (int)ring.size(); ++i)
			{
				long long c0, c1;
				c0 = max(l0+f[ring[i]][0], l1+f[ring[i]][0]);
				c1 = l0+f[ring[i]][1];
				l0 = c0, l1 = c1;
			}
			ans0 = max(ans0, l0), ans1 = max(ans1, l1);
			l0 = -INF, l1 = f[ring[0]][1];
			for(int i = 1; i < (int)ring.size(); ++i)
			{
				long long c0, c1;
				c0 = max(l0+f[ring[i]][0], l1+f[ring[i]][0]);
				c1 = l0+f[ring[i]][1];
				l0 = c0, l1 = c1;
			}
			ans0 = max(ans0, l0);
			f[now][0] = ans0, f[now][1] = ans1;
		}
	for(set<int>::iterator it = s.begin(); it != s.end(); ++it)
	{
		f[now][0] += max(f[*it][0], f[*it][1]);
		f[now][1] += f[*it][0];		
	}
}

int main()
{
	freopen("bzoj1040.in", "r", stdin);
	freopen("bzoj1040.out", "w", stdout);
	
	read(n);
	for(int i = 1; i <= n; ++i)
	{
		int to;
		read(w[i]), read(to);
		G.add_edge(i, to);	
	}
	for(int i = 1; i <= n; ++i)
		if(!dfn[i])
		{
			dfs(i);
			ans += max(f[i][0], f[i][1]);
		}
	cout << ans << endl;
	
	return 0;
}
