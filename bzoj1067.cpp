#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 5e4+9;

int n, m;
pair<int,int> a[N];

int val[N], tot;

int tree[N<<2];

inline void init()
{
	scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		scanf("%d%d", &a[i].first, &a[i].second);
		val[++tot] = a[i].first;	
	}
}

inline int id(int x)
{
	return lower_bound(val+1, val+tot+1, x)-val;
}

inline void maintain(int x)
{
	tree[x] = max(tree[x<<1], tree[x<<1|1]);
}

void insert(int x, int l, int r, int pos, int val)
{
	if(l == r)
	{
		tree[x] = val;
		return ;
	}
	int mid = (l+r)>>1;
	if(pos <= mid) insert(x<<1, l, mid, pos, val);
	else insert(x<<1|1, mid+1, r, pos, val);
	maintain(x);
}

inline int query(int x, int l, int r, int lp, int rp)
{
	if(rp < lp) return 0;
	if(l == lp && r == rp) return tree[x];
	int mid = (l+r)>>1;
	if(rp <= mid) return query(x<<1, l, mid, lp, rp);
	else if(lp > mid) return query(x<<1|1, mid+1, r, lp, rp);
	else return max(query(x<<1, l, mid, lp, mid), query(x<<1|1, mid+1, r, mid+1, rp));
}

inline void solve()
{
	for(int i = 1; i <= n; ++i)
		insert(1, 1, tot, i, a[i].second);
	scanf("%d", &m);
	while(m--)
	{
		int y, x;
		scanf("%d%d", &y, &x);
		int idx = id(x), idy = id(y);
		int midv = query(1, 1, tot, idy+1, idx-1);
		if(val[idx] == x && val[idy] == y)
		{
			if(y-x+1 == idy-idx+1)
			{
				if(a[idx].second <= a[idy].second && midv < a[idx].second)
					puts("true");
				else puts("false");
			}
			else 
			{
				if(a[idx].second <= a[idy].second && midv < a[idx].second)
					puts("maybe");
				else puts("false");
			}
		}
		else if(val[idx] == x)
		{
			if(idy < idx)
				midv = max(midv, a[idy].second);
			if(midv < a[idx].second) 
				puts("maybe");
			else puts("false");
		} 
		else if(val[idy] == y)
		{
			if(idx == idy+1)
				puts("maybe");
			else if(midv < a[idy].second)
				puts("maybe");
			else puts("false");
		}
		else puts("maybe");
	}
}

int main()
{
	freopen("bzoj1067.in", "r", stdin);
	freopen("bzoj1067.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
