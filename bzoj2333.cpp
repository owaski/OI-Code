#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <set>
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
char Out[20] = "\0";

template<class T>
inline void write(T x)
{
	if(x < 0) putchar('-'), x = -x;
	else if(!x) {putchar('0');return ;}
	OutN = 0;
	while(x)
	{
		Out[OutN++] = x%10+'0';
		x /= 10;
	}
	while(OutN--)
		putchar(Out[OutN]);
}

const int N = 300009;

int n, m;
int a[N];
multiset<int> val;
int allAdd;

namespace Dsu
{
	int rk[N];
	int fa[N];
	int add[N];
	
	inline void init()
	{
		for(int i = 1; i <= n; ++i)
			rk[i] = 1, fa[i] = i, add[i] = 0;
	}
	
	inline int findF(int x)
	{
		while(fa[x] != x)
			x = fa[x];
		return x;
	}
	
	inline int findA(int x)
	{
		int val = add[x];
		while(x != fa[x])
		{
			x = fa[x];
			val += add[x];
		}
		return val;
	}
	
	inline void merge(int x, int y)
	{
		int fx = findF(x), fy = findF(y);
		if(rk[fx] > rk[fy]) swap(fx, fy);
		fa[fx] = fy, add[fx] -= add[fy];
		rk[fy] = max(rk[fy], rk[fx]+1);
	}
}

namespace MergableHeap
{
	int dep[N];
	int key[N];
	int fa[N];
	int lc[N];
	int rc[N];
	
	inline void init()
	{
		for(int i = 1; i <= n; ++i)
		{
			key[i] = a[i], dep[i] = 0;
			fa[i] = lc[i] = rc[i] = 0;
		}
	}
	
	inline int findR(int x)
	{
		while(fa[x])
			x = fa[x];
		return x;
	}
	
	inline int getKey(int x)
	{
		return key[x]+Dsu::findA(x);
	}
	
	inline int merge(int x, int y)
	{
		if(!x) return y;
		if(!y) return x;
		if(getKey(x) < getKey(y)) 
			swap(x, y);
		rc[x] = merge(rc[x], y);
		fa[rc[x]] = x;
		if(dep[lc[x]] < dep[rc[x]])
			swap(lc[x], rc[x]);
		dep[x] = dep[rc[x]]+1;
		return x;
	}
	
	inline int del(int x)
	{
		int tmp = merge(lc[x], rc[x]);
		fa[tmp] = 0;
		if(lc[fa[x]] == x) 
			lc[fa[x]] = 0;
		else rc[fa[x]] = 0;
		int ret = merge(findR(fa[x]), tmp);
		fa[x] = lc[x] = rc[x] = 0;
		return ret;
	}
}

inline void init()
{
	read(n);
	for(int i = 1; i <= n; ++i)
	{
		read(a[i]);
		val.insert(a[i]);
	}
	Dsu::init();
	MergableHeap::init();
}

inline void solve()
{
	read(m);
	int x, y;
	char c[3] = "\0";
	while(m--)
	{
		scanf("%s", c);
		if(c[0] == 'U')
		{
			read(x), read(y);
			if(Dsu::findF(x) != Dsu::findF(y))
			{
				Dsu::merge(x, y);
				int Hrx = MergableHeap::findR(x);
				int Hry = MergableHeap::findR(y);
				val.erase(val.lower_bound(MergableHeap::getKey(Hrx)));
				val.erase(val.lower_bound(MergableHeap::getKey(Hry)));
				val.insert(MergableHeap::getKey(MergableHeap::merge(Hrx, Hry)));
			}
		}
		else if(c[0] == 'A')
		{
			if(c[1] == '1')
			{
				read(x), read(y);
				int Hrx = MergableHeap::findR(x);
				val.erase(val.lower_bound(MergableHeap::getKey(Hrx)));
				Hrx = MergableHeap::del(x);
				MergableHeap::key[x] += y;
				val.insert(MergableHeap::getKey(MergableHeap::merge(x, Hrx)));
			}
			else if(c[1] == '2')
			{
				read(x), read(y);
				int Dfx = Dsu::findF(x);
				int Hrx = MergableHeap::findR(x);
				val.erase(val.lower_bound(MergableHeap::getKey(Hrx)));
				Dsu::add[Dfx] += y, val.insert(MergableHeap::getKey(Hrx));
			}
			else
			{
				read(y);
				allAdd += y;
			}
		}
		else
		{
			if(c[1] == '1')
			{
				read(x);
				write(MergableHeap::getKey(x)+allAdd);
				putchar('\n');
			}
			else if(c[1] == '2')
			{
				read(x);
				int Hrx = MergableHeap::findR(x);
				write(MergableHeap::getKey(Hrx)+allAdd);
				putchar('\n');
			}
			else 
			{
				multiset<int>::iterator it = val.end();--it;
				write(*it+allAdd); 
				putchar('\n');
			}
		}
	}
}

int main()
{
	freopen("bzoj2333.in", "r", stdin);
	freopen("bzoj2333.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
