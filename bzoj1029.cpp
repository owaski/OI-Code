#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
#define pii pair<int,int>
#define X first
#define Y second
#define mp make_pair
using namespace std;

template<class T> inline void read(T &x)
{
	char c;x = 0;
	while((c=getchar()) < '0' || c > '9');
	x = c-'0';
	while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
}

const int MAXN = 150009;

int n;
pii t[MAXN];

struct Heap
{
	int sz;
	int val[MAXN];
	inline void Swap(int x, int y)
	{
		swap(val[x], val[y]);
	}
	void up(int x)
	{
		int y = x>>1;
		if(y && val[x] >= val[y])
		{
			Swap(x, y);
			up(y);
		}
	}
	void down(int x)
	{
		int y = x<<1, z = y|1;
		if(y <= sz && val[y] >= val[x] && (z > sz || val[y] >= val[z]))
		{	
			Swap(x, y);
			down(y);
		}
		else if(z <= sz && val[z] >= val[x] && val[z] >= val[y])
		{
			Swap(x, z);
			down(z);
		}
	}
	inline void insert(int x)
	{
		val[++sz] = x;
		up(sz);
	}
	inline void remove()
	{
		Swap(1, sz);
		val[sz--] = 0;
		down(1);
	}
	inline int top()
	{
		return val[1];
	}
}heap;

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1029.in", "r", stdin);
	freopen("bzoj1029.out", "w", stdout);
#endif

	read(n);
	rep(i, 1, n)
		read(t[i].Y), read(t[i].X);
	sort(t+1, t+n+1);
	int cur = 0;
	int ans = 0;
	rep(i, 1, n)
		if(cur+t[i].Y <= t[i].X)
		{
			cur += t[i].Y;
			heap.insert(t[i].Y);
			ans++;
		}
		else if(heap.top() > t[i].Y)
		{
			cur -= heap.top()-t[i].Y;
			heap.remove(), heap.insert(t[i].Y);
		}
	printf("%d\n", ans);

	return 0;
}
