#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
#define clr(a, b) memset(a, b, sizeof a)
typedef double DB;
using namespace std;

const int MAXN = 509;
const DB eps = 1e-8;

struct Pot
{
	DB x, y;
	Pot() {x = y = 0;}
	Pot(DB _x, DB _y):x(_x),y(_y) {}
	
	inline void input()
	{
		DB z;
		scanf("%lf%lf%lf", &x, &y, &z);
	}
	inline friend Pot operator + (const Pot &a, const Pot &b)
	{
		return Pot(a.x+b.x, a.y+b.y);
	}
	inline friend Pot operator - (const Pot &a, const Pot &b)
	{
		return Pot(a.x-b.x, a.y-b.y);
	}
	inline friend DB operator * (const Pot &a, const Pot &b)
	{
		return a.x*b.x+a.y*b.y;
	}
	inline friend DB operator ^ (const Pot &a, const Pot &b)
	{
		return a.x*b.y-a.y*b.x;
	}
	inline friend bool operator != (const Pot &a, const Pot &b)
	{
		return fabs(a.x-b.x) > eps || fabs(a.y-b.y) > eps; 
	}
};

int m, n;
Pot p[MAXN], q[MAXN];
Pot t[MAXN], st[MAXN];
int top;
int g[MAXN][MAXN];

inline DB calc(const Pot &a, const Pot &b, const Pot &c)
{
	return ((b-a)^(c-a));
}

inline bool online(const Pot &a, const Pot &b, const Pot &c)
{
	return fabs((b-a)^(c-a)) <= eps && c.x >= min(a.x, b.x)-eps && c.x <= max(a.x, b.x)+eps;
}

inline bool check(int a, int b)
{
	Pot p = ::p[a], q = ::p[b];
	rep(i, 1, n)
	{
		DB tmp = calc(p, q, ::q[i]);
		if(fabs(tmp) <= eps)
		{
			if(!online(p, q, ::q[i]))
				return false;
		}
		else if(tmp < -eps)
			return false;
	}
	return true;
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("metal.in", "r", stdin);
	freopen("metal.out", "w", stdout);
	#endif
	
	scanf("%d%d", &m, &n);
	rep(i, 1, m) p[i].input();
	rep(i, 1, n) q[i].input();
	rep(i, 1, m)
	{
		bool flag = true;
		rep(j, 1, n)
			if(q[j] != p[i])
			{
				flag = false;
				break;
			}
		if(flag)
		{
			puts("1");
			return 0;
		}
	}
	rep(i, 1, m)
		rep(j, i+1, m)
		{
			bool flag = true;
			rep(k, 1, n)
				if(!online(p[i], p[j], q[k]))
				{
					flag = false;
					break;
				}
			if(flag)
			{
				puts("2");
				return 0;
			}
		}
	rep(i, 1, m)
		rep(j, 1, m)
			g[i][j] = MAXN;
	rep(i, 1, m)
		rep(j, 1, m)
			if(i != j && check(i, j))
				g[i][j] = 1;
	rep(k, 1, m)
		rep(i, 1, m)
			rep(j, 1, m)
				if(g[i][j] > g[i][k]+g[k][j])
					g[i][j] = g[i][k]+g[k][j];
	int ans = MAXN;
	rep(i, 1, m)
		ans = min(ans, g[i][i]);
	if(ans == MAXN) puts("-1");
	else printf("%d\n", ans);
	
	return 0;
}
