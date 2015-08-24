#include <bits/stdc++.h>
#define pii pair<int,int>
#define X first
#define Y second
#define pb push_back
#define mp make_pair
#define clr(a, b) memset(a, b, sizeof a)
#define rep(i, a, b) for(register int i = (a); i <= (b); ++i)
#define per(i, a, b) for(register int i = (a); i >= (b); --i)
#define debug(...) fprintf(stderr, __VA_ARGS__)
typedef unsigned long long ULL;
typedef long long LL;
typedef double DB;
typedef long double LD;
using namespace std;

template<class T> void read(T &x)
{
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}
template<class T> void prt(T x)
{
    if(!x) putchar('0');
    else
    {
    	if(x < 0)
    	{
    		putchar('-');
    		x = -x;
		}
        int tot = 0;
        char s[20] = "\0";
        while(x)
        {
            s[++tot] = x%10+'0';
            x /= 10;
        }
        per(i, tot, 1)
            putchar(s[i]);
    }
}

const int MAXN = 1e5+9;
const int MAXM = 2e5+9;
const int INF = 1e9;

int n, m;
int sz = 1, head[MAXN], to[MAXM], ne[MAXM];
int deg[MAXN];
bool vis[MAXN];
int dfn[MAXN], tot;
int low[MAXN];
bool ans[MAXN];
int owaski;

inline void add_edge(int u, int v)
{
	to[sz] = v, ne[sz] = head[u], head[u] = sz++;
	to[sz] = u, ne[sz] = head[v], head[v] = sz++;
}

void dfs(int x, int fa)
{
	vis[x] = true;
	dfn[x] = low[x] = ++tot;
	bool flag = false;int cnt = 0;
	for(register int i = head[x]; i; i = ne[i])
		if(to[i] != fa)
			if(vis[to[i]]) low[x] = min(low[x], dfn[to[i]]);
			else 
			{
				cnt++;
				dfs(to[i], x), low[x] = min(low[x], low[to[i]]);
				if(low[to[i]] >= dfn[x]) flag = true;
			}
	if(deg[x] == m-n+2 && (!flag || x == 1 && cnt == 1)) ans[x] = true, owaski++;
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("cancer.in", "r", stdin);
	freopen("cancer.out", "w", stdout);
	#endif
	
	read(n), read(m);
	rep(i, 1, m)
	{
		int u, v;
		read(u), read(v);
		add_edge(u, v);
		deg[u]++, deg[v]++;
	}
	rep(i, 1, n)
		if(!deg[i])
		{
			puts("1");
			prt(i), puts("");
			return 0;
		}
	dfs(1, 0);
	prt(owaski), puts("");
	rep(i, 1, n) 
		if(ans[i])
			prt(i), putchar(' ');

	return 0;
}
