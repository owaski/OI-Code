#include <bits/stdc++.h>
using namespace std;

const double INF = 1e12;
const double eps = 1e-9;
const int MAXN = 309;

struct Pot
{
	double x, y;
	Pot(double _x = 0, double _y = 0):x(_x), y(_y) {}
	inline friend Pot operator + (const Pot &a, const Pot &b)
	{
		return Pot(a.x+b.x, a.y+b.y);
	}
	inline friend Pot operator - (const Pot &a, const Pot &b)
	{
		return Pot(a.x-b.x, a.y-b.y);
	}
	inline friend double operator * (const Pot &a, const Pot &b)
	{
		return a.x*b.x+a.y*b.y;
	}
	inline friend double operator ^ (const Pot &a, const Pot &b)
	{
		return a.x*b.y-a.y*b.x;
	}
	inline friend Pot operator * (const Pot &a, const double &k)
	{
		return Pot(a.x*k, a.y*k);
	}
	inline friend Pot operator / (const Pot &a, const double &k)
	{
		return Pot(a.x/k, a.y/k);
	}
	inline friend bool operator < (const Pot &a, const Pot &b)
	{
		if(a.x == b.x) return a.y < b.y;
		else return a.x < b.x;
	}
};

struct Line
{
	Pot p, v;
	double ang;
	Line() {}
	Line(Pot _p, Pot _q)
	{
		p = _p, v = _q-p;
		ang = atan2(v.y, v.x);
	}
	inline bool on_left(const Pot &a)
	{
		return (v^(a-p)) >= 0;
	}
	inline friend bool operator < (const Line &a, const Line &b)
	{
		return a.ang < b.ang;
	}
};

int n, m;
Pot node[MAXN];
Pot p[MAXN];
Line l[MAXN];
Line q[MAXN];
int h, t;
double ans;

inline void init_plane()
{
	l[++m] = Line(Pot(node[1].x,INF),Pot(node[1].x,0));
	l[++m] = Line(Pot(node[n].x,INF),Pot(node[1].x,INF));
	l[++m] = Line(Pot(node[n].x,0),Pot(node[n].x,INF));
	l[++m] = Line(Pot(node[1].x,0),Pot(node[n].x,0));
}

inline Pot get_line_intersection(const Line &a, const Line &b)
{
	Pot w = b.p-a.p;
	double k = (w^a.v)/(a.v^b.v);
	return b.p+b.v*k;
}

inline void half_plane_intersection()
{
	q[h=t=0] = l[1];
	for(int i = 2; i <= m; ++i)
	{
		while(h < t && !l[i].on_left(p[t-1])) --t;
		while(h < t && !l[i].on_left(p[h])) ++h;
		if(fabs(l[i].ang-q[t].ang) <= eps)
		{
			if(!l[i].on_left(p[t-1]))
				q[t] = l[i];
		}
		else q[++t] = l[i];
		if(h < t) p[t-1] = get_line_intersection(q[t-1], q[t]);
	}
	bool end = false;
	while(!end)
	{
		end = true;
		while(h < t && !q[t].on_left(p[h])) ++h, end = false;
		while(h < t && !q[h].on_left(p[t-1])) --t, end = false;
	}
	if(h < t) p[t] = get_line_intersection(q[t], q[h]);
}

inline double gety(const Pot &a, const Pot &b, const double &x)
{
	if(a.x == b.x) return min(a.y, b.y);
	else return (a+(b-a)*(x-a.x)/(b.x-a.x)).y;
}

inline void get_ans()
{
	int f = h+1;
	sort(p+h, p+t);
	for(int i = 1; i <= n; ++i)
	{
		while(f < t-1 && p[f].x < node[i].x) ++f;
		ans = min(ans, gety(p[f-1], p[f], node[i].x)-node[i].y);
	}
	f = 2;
	for(int i = h; i < t; ++i)
	{
		while(f < n && node[f].x < p[i].x) ++f;
		ans = min(ans, p[i].y-gety(node[f-1], node[f], p[i].x));
	}
}	

int main()
{
	freopen("bzoj1038.in", "r", stdin);
	freopen("bzoj1038.out", "w", stdout);
	
	scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
		scanf("%lf", &node[i].x);
	for(int i = 1; i <= n; ++i)
		scanf("%lf", &node[i].y);
	init_plane();
	for(int i = 1; i < n; ++i)
		l[++m] = Line(node[i], node[i+1]);
	sort(l+1, l+m+1);
	half_plane_intersection();
	ans = INF, get_ans();
	printf("%.3lf\n", ans);
	
	return 0;
}
