#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
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

int OutN;
int Out[20];

template<class T>
inline void write(T x)
{
	if(x < 0) putchar('-'), x = -x;
	else if(x == 0) {puts("0");return ;}
	OutN = 0;
	while(x)
	{
		Out[++OutN] = x%10+'0';
		x /= 10;	
	}
	while(OutN--)
		putchar(Out[OutN+1]);
	puts("");
}

const int N = 100009;
const int M = N<<1;
const int INF = 0x3f3f3f3f;

int n, m, MOD;
struct Graph
{
	int sz;
	int head[N];
	int to[M];
	int ne[M];
	Graph()
	{
		sz = 1;
		memset(head, 0, sizeof head);
	}
	inline void add_edge(int u, int v)
	{
		to[sz] = v, ne[sz] = head[u], head[u] = sz++;
	}
}G;

int fa[N];
bool vis[N];
int q[N], l, r;
long long f[N][11][3];

inline void init()
{
	read(n), read(m), read(MOD);
	for(int i = 1; i <= m; ++i)
	{
		int u, v;
		read(u), read(v);
		G.add_edge(u, v);
		G.add_edge(v, u);
	}
}

inline void bfs()
{
	q[++r] = 1, vis[1] = true;
	while(l < r)
	{
		int now = q[++l];
		for(int i = G.head[now]; i; i = G.ne[i])
			if(!vis[G.to[i]])
			{
				fa[G.to[i]] = now;
				vis[G.to[i]] = true;
				q[++r] = G.to[i];
			}
	}
}

template<class T>
inline T mod(T x)
{
	return x==0?0:x%MOD==0?MOD:x%MOD;
}

inline void dp()
{
	for(int i = r; i >= 1; --i)
	{
		int now = q[i];
		for(int j = 0; j <= 10; ++j)
			f[now][j][0] = 1;
		for(int j = G.head[now]; j; j = G.ne[j])
			if(G.to[j] != fa[now])
			{
				int to = G.to[j];
				for(int k = 0; k <= 10; ++k)
				{
					long long v0 = f[now][k][0];
					long long v1 = f[now][k][1];
					long long v2 = f[now][k][2];
					long long f0 = k?mod(f[to][k-1][0]+f[to][k-1][1]+f[to][k-1][2]):0;
					long long f1 = mod(f[to][k][0]+f[to][k][1]);
					f[now][k][0] = mod(v0*f0);
					f[now][k][1] = mod(v1*f0+v0*f1);
					f[now][k][2] = mod(v2*f0+v1*f1);
				}
			}
	}
	for(int i = 0; i <= 10; ++i)
		printf("%lld %lld %lld\n", f[1][i][0], f[1][i][1], f[1][i][2]);
}

inline void solve()
{
	bfs();
	if(r < n) 
	{
		puts("-1"), puts("-1");
		return ;
	}
	dp();
	long long ans = 0;
	for(int i = 0; i <= 10; ++i)
		if((ans = f[1][i][0]+f[1][i][1]+f[1][i][2]) > 0)
		{
			cout << i << endl;
			cout << ans%MOD << endl;
			return ;
		}
}

int main()
{
	freopen("bzoj1063.in", "r", stdin);
	freopen("bzoj1063.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
