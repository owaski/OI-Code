#include <cmath>
#include <cstdio>
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

int OutN;
char Out[20];

template<class T>
inline void write(T x)
{
	if(x < 0) putchar('-'), x = -x;
	if(x)
	{
		OutN = 0;
		while(x)
		{
			Out[OutN++] = x%10+'0';
			x /= 10;
		}
		while(OutN--)
			putchar(Out[OutN]);
	}
	else putchar('0');
}

const int N = 70009;
const int V = 70000;
const int LOGN = 20;
const int SZ = 20000000;
const double ALPHA = 0.75;
const double LOGALPHA = log(4.0)-log(3.0);

int n, v[N];

namespace SegmentTree
{
	int lc[SZ], rc[SZ], sum[SZ];
	int Stack[SZ], Stop;
	
	void init(int x)
	{
		lc[x] = rc[x] = sum[x] = 0;
	}
	
	int newNode()
	{
		int ret = Stack[Stop--];
		init(ret);return ret;
	}
	
	void insert(int &x, int l, int r, int pos, int v)
	{
		if(x == 0) 
			x = newNode();
		sum[x] += v;
		if(l == r) 
			return ;
		int mid = (l+r)>>1;
		if(pos <= mid) insert(lc[x], l, mid, pos, v);
		else insert(rc[x], mid+1, r, pos, v);
	}
	
	void newTree(int &x, int pos)
	{
		x = newNode(), sum[x]++;
		int l = 0, r = V, cur = x;
		while(l < r)
		{
			int mid = (l+r)>>1;
			if(pos <= mid) cur = lc[cur] = newNode(), r = mid;
			else cur = rc[cur] = newNode(), l = mid+1;
			sum[cur]++;
		}
	}
	
	void merge(int &x, int y)
	{
		if(y == 0) 
			return ;
		if(x == 0)
			x = newNode();
		sum[x] += sum[y];
		merge(lc[x], lc[y]);
		merge(rc[x], rc[y]);
	}
	
	void clear(int &x)
	{
		Stack[++Stop] = x;
		if(lc[x]) clear(lc[x]);
		if(rc[x]) clear(rc[x]);
		init(x), x = 0;
	}
	/*
	void Debug(int x, int l, int r)
	{
		if(l == r)
		{
			debug("%d : %d\n", l, sum[x]);
			return ;
		}
		int mid = (l+r)>>1;
		if(lc[x]) Debug(lc[x], l, mid);
		if(rc[x]) Debug(rc[x], mid+1, r);
	}*/
}

namespace ScapgoatTree
{
	int root;
	int lc[N], rc[N], sz[N], rt[N];
	int dfn[N], dfnN, val[N], valN;
	int maxDepth;
	
	int build(int l, int r)
	{
		int mid = (l+r)>>1, x = dfn[mid];
		SegmentTree::newTree(rt[x], v[x]);
		if(l < mid) lc[x] = build(l, mid-1);
		if(mid < r) rc[x] = build(mid+1, r);
		sz[x] = sz[lc[x]]+sz[rc[x]]+1;
		SegmentTree::merge(rt[x], rt[lc[x]]);
		SegmentTree::merge(rt[x], rt[rc[x]]);
		return x;
	}
	
	void get(int x)
	{
		if(lc[x]) get(lc[x]);
		dfn[++dfnN] = x;
		if(rc[x]) get(rc[x]);
	}
	
	int rebuild(int x)
	{
		dfnN = 0, get(x);
		for(int i = 1; i <= dfnN; ++i)
		{
			SegmentTree::clear(rt[dfn[i]]);
			lc[dfn[i]] = rc[dfn[i]] = sz[dfn[i]] = 0;
		}
		return build(1, dfnN);
	}
	
	void get(int x, int l, int r, int ql, int qr)
	{
		if(ql <= l && r <= qr)
		{
			dfn[++dfnN] = rt[x];
			return ;
		}
		int mid = l+sz[lc[x]];
		if(ql < mid && lc[x]) get(lc[x], l, mid-1, ql, qr);
		if(ql <= mid && mid <= qr) val[++valN] = v[x];
		if(qr > mid && rc[x]) get(rc[x], mid+1, r, ql, qr);
	}
	
	int query(int l, int r, int k)
	{
		valN = dfnN = 0, get(root, 1, n, l, r);
		l = 0, r = V;
		while(l < r)
		{
			int ls = 0, mid = (l+r)>>1;
			for(int i = 1; i <= dfnN; ++i)
				ls += SegmentTree::sum[SegmentTree::lc[dfn[i]]];
			for(int i = 1; i <= valN; ++i)
				if(l <= val[i] && val[i] <= mid) ls++;
			if(ls >= k) 
			{
				r = mid;
				for(int i = 1; i <= dfnN; ++i)
					dfn[i] = SegmentTree::lc[dfn[i]];
			}
			else
			{
				k -= ls, l = mid+1;
				for(int i = 1; i <= dfnN; ++i)
					dfn[i] = SegmentTree::rc[dfn[i]];
			}
		}
		return l;
	}
	
	void get(int x, int pos)
	{
		dfn[++dfnN] = x;
		if(sz[lc[x]] >= pos) get(lc[x], pos);
		else if(sz[lc[x]]+1 == pos) return ;
		else get(rc[x], pos-sz[lc[x]]-1);
	}
	
	void modify(int x, int value)
	{
		dfnN = 0, get(root, x);
		for(int i = 1; i <= dfnN; ++i)
		{
			SegmentTree::insert(rt[dfn[i]], 0, V, v[dfn[dfnN]], -1);
			SegmentTree::insert(rt[dfn[i]], 0, V, value, 1);
		}
		v[dfn[dfnN]] = value;
	}
	
	bool insert(int &x, int pos, int p, int d)
	{
		if(x == 0)
		{
			sz[x = p]++;
			SegmentTree::newTree(rt[x], v[x]);
			return d <= maxDepth;
		}
		sz[x]++;
		SegmentTree::insert(rt[x], 0, V, v[p], 1);
		bool ret;
		if(pos <= sz[lc[x]]+1) ret = insert(lc[x], pos, p, d+1);
		else ret = insert(rc[x], pos-sz[lc[x]]-1, p, d+1);
		int lim = (int)(sz[x]*ALPHA);
		if(ret && (sz[lc[x]] > lim || sz[rc[x]] > lim))
		{
			x = rebuild(x);
			return false;
		}
		else return ret;
	}
	
	void insert(int pos, int p)
	{
		maxDepth = log(1.0*n)/LOGALPHA;
		insert(root, pos, p, 0);
	}
	/*
	void Debug(int x)
	{
		if(lc[x]) Debug(lc[x]);
		debug("%d ", v[x]);
		if(rc[x]) Debug(rc[x]);
	}*/
}

void init()
{
	read(n);
	for(int i = 1; i <= n; ++i)
	{
		read(v[i]);
		ScapgoatTree::dfn[i] = i;
	}
	for(int i = 1; i < SZ; ++i)
		SegmentTree::Stack[++SegmentTree::Stop] = SZ-i;
	ScapgoatTree::root = ScapgoatTree::build(1, n); 
}

void solve()
{
	int qN;
	read(qN);
	int lastAns = 0;
	while(qN--)
	{
		int x, y, z;
		char str[4] = "\0";
		scanf("%s", str);
		switch(str[0])
		{
			case 'Q':
				read(x), read(y), read(z);
				x ^= lastAns, y ^= lastAns, z ^= lastAns;
				write(lastAns = ScapgoatTree::query(x, y, z)), putchar('\n');
				break;
			case 'M':
				read(x), read(y);
				x ^= lastAns, y ^= lastAns;
				ScapgoatTree::modify(x, y);
				break;
			default:
				read(x), read(y);
				x ^= lastAns, y ^= lastAns, v[++n] = y;
				ScapgoatTree::insert(x, n);
		}
	}
}

int main()
{
	freopen("bzoj3065.in", "r", stdin);
	freopen("bzoj3065.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
