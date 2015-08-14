#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <set>
#include <list>
#include <stack>
#include <queue>
#include <vector>
#define pii pair<int,int>
#define X first
#define Y second
#define pb push_back
#define mp make_pair
#define clr(a, b) memset(a, b, sizeof a)
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
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

const int MAXN = 109;
const int MAXM = 10009;
const DB INF = 1e15;

int n, m;
DB x[MAXN], y[MAXN];
struct Edge{int u,v;DB w;}E[MAXM];
DB in[MAXN];
int id[MAXN];
int fa[MAXN];
int vis[MAXN];

inline DB sqr(DB x)
{
	return x*x;
}
inline DB dis(int i, int j)
{
	return sqrt(sqr(x[i]-x[j])+sqr(y[i]-y[j]));
}

DB directed_mst(int r)
{
	DB ret = 0;
	while(true)
	{
		clr(id, 0);
		clr(fa, 0);
		clr(vis, 0);
		rep(i, 1, n)
			in[i] = INF;
		rep(i, 1, m)
		{
			int u = E[i].u;
			int v = E[i].v;
			DB d = E[i].w;
			if(in[v] > d && u != v /*It's important*/)
				in[v] = d, fa[v] = u;
		}
		rep(i, 1, n)
			if(i != r && !fa[i])
				return -1;
		fa[r] = 0;
		int tot = 0;
		id[r] = ++tot;
		rep(i, 2, n)
		{
			ret += in[i];
			if(!vis[i])
			{
				int now = i;
				while(now != r && !vis[now])
					vis[now] = i, now = fa[now];
				if(now != r && vis[now] == i)
				{
					++tot;int tmp = now;
					while(fa[tmp] != now)
						id[tmp] = tot, tmp = fa[tmp];
					id[tmp] = tot;
					tmp = i;
					while(tmp != now)
						id[tmp] = ++tot, tmp = fa[tmp];
					/*Maybe it is a loop like 'p'*/
				}
				else
				{
					now = i;
					while(now != r && vis[now] == i)
						id[now] = ++tot, now = fa[now];
				}
			}
		}
		if(tot == n)
			break;
		rep(i, 1, m)
		{
			int v = E[i].v;
			E[i].u = id[E[i].u];
			E[i].v = id[v];
			if(E[i].u != E[i].v /*It's important*/)
				E[i].w -= in[v];
		}
		r = id[r];
		n = tot;
	}
	return ret;
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	#endif

	while(scanf("%d%d", &n, &m) != EOF)
	{
		rep(i, 1, n)
			scanf("%lf%lf", x+i, y+i);
		rep(i, 1, m)
		{
			int u, v;
			scanf("%d%d", &u, &v);
			E[i].u = u;
			E[i].v = v;
			E[i].w = dis(u, v);
		}
		DB ans = directed_mst(1);
		if(ans == -1) puts("poor snoopy");
		else printf("%.2lf\n", ans);
	}

	#ifndef ONLINE_JUDGE
	fclose(stdin);
	fclose(stdout);
	#endif
	return 0;
}
