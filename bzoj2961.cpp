#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 500009;

enum Ans {Yes, No, Unknown};

int n;

struct Pot
{
	double x, y;
	Pot() {}
	Pot(double _x, double _y):x(_x), y(_y) {}	
	inline void input()
	{
		scanf("%lf%lf", &x, &y);	
	}
	inline friend Pot operator + (const Pot &a, const Pot &b)
	{
		return Pot(a.x+b.x, a.y+b.y);	
	}
	inline friend Pot operator - (const Pot &a, const Pot &b)
	{
		return Pot(a.x-b.x, a.y-b.y);	
	}
	inline friend Pot operator * (const Pot &a, const double &k)
	{
		return Pot(a.x*k, a.y*k);
	}
	inline friend Pot operator / (const Pot &a, const double &k)
	{
		return Pot(a.x/k, a.y/k);	
	}
	inline friend double operator * (const Pot &a, const Pot &b)
	{
		return a.x*b.x+a.y*b.y;	
	}
	inline friend double operator ^ (const Pot &a, const Pot &b)
	{
		return a.x*b.y-a.y*b.x;
	}	
	inline friend bool operator < (const Pot &a, const Pot &b)
	{
		return a.x < b.x || (a.x == b.x && a.y > b.y);
	}
}st[N];
int top;

struct Query
{
	int type, i;
	Pot p, k;
	inline void input()
	{
		scanf("%d", &type);
		p.input();
		if(type == 1) 
		{
			k.x = 1;
			k.y = -p.x/p.y;
			p = Pot(0, (p.x*p.x+p.y*p.y)/(2*p.y));
		}
	}
}q[N], u[N], v[N], f[N], g[N];
Ans ans[N];

inline bool on_left(const Pot &a, const Pot &p, const Pot &v)
{
	return (v^(a-p)) >= 0;
}

inline double cmp(const Pot &a, const Pot &b)
{
	return b^a;	
}

void divide_and_conquer(int l, int r)
{
	if(l == r) return ;
	int mid = (l+r)>>1;
	divide_and_conquer(l, mid);
	divide_and_conquer(mid+1, r);
	top = 0;int last = -1, t1 = 0, t2 = 0;
	for(int i = l; i <= mid; ++i)
		if(q[i].type == 0)
		{
			u[++t1] = q[i];
			while(top > 1 && ((st[top]-q[i].p)^(st[top-1]-q[i].p)) >= 0) --top;	
			st[++top] = q[i].p, last = t1;
		}
	int tmp = top;
	for(int i = t1; i >= 1; --i)
		if(i != last)
		{
			while(top > tmp && ((st[top]-u[i].p)^(st[top-1]-u[i].p)) >= 0) --top;
			st[++top] = u[i].p;	
		}
	if(top > 1) top--;
	if(top)
	{
		if(top <= 2)
		{
			for(int j = 1; j <= top; ++j)
				for(int i = mid+1; i <= r; ++i)
					if(q[i].type == 1)
						if(!on_left(st[j], q[i].p, q[i].k))
							ans[q[i].i] = No;
		}
		else
		{
			int cur = 1;
			for(int i = mid+1; i <= r; ++i)
				if(q[i].type == 1)
				{
					while(!(cmp(st[cur%top+1]-st[cur], q[i].k) <= 0 && cmp(st[(cur+1)%top+1]-st[cur%top+1], q[i].k) >= 0)) cur = cur%top+1;
					if(!on_left(st[cur%top+1], q[i].p, q[i].k)) ans[q[i].i] = No; 
				}
		}
	}
	for(int i = mid+1; i <= r; ++i)
		if(q[i].type == 0)
			v[++t2] = q[i];
	int t3 = 0, t4 = 0;
	for(int i = l; i <= mid; ++i)
		if(q[i].type == 1)
			f[++t3] = q[i];
	for(int i = mid+1; i <= r; ++i)
		if(q[i].type == 1)
			g[++t4] = q[i];
	int t = l;
	for(int i = 1, j = 1; i <= t1 || j <= t2;)
	{
		if(i <= t1 && (j > t2 || u[i].p < v[j].p)) q[t++] = u[i++];
		else q[t++] = v[j++];	
	}
	for(int i = 1, j = 1; i <= t3 || j <= t4;)
	{
		if(i <= t3 && (j > t4 || cmp(f[i].k, g[j].k) >= 0)) q[t++] = f[i++];
		else q[t++] = g[j++];
	}
}


int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj2961.in", "r", stdin);
	freopen("bzoj2961.out", "w", stdout);
#endif

	scanf("%d", &n);
	bool flag = true;
	for(int i = 1; i <= n; ++i)
	{
		q[i].input();
		q[i].i = i, ans[i] = Unknown;
		if(q[i].type) 
		{
			if(flag) ans[i] = No;
			else ans[i] = Yes;
		}
		else flag = false;
	}
	divide_and_conquer(1, n);
	for(int i = 1; i <= n; ++i)
		if(ans[i] == Yes)  puts("Yes");
		else if(ans[i] == No) puts("No");

	return 0;
}
