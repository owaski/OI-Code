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
int Out[20];

template<class T>
inline void write(T x)
{
	if(x < 0) putchar('-'), x = -x;
	else if (!x) {puts("0");return ;}
	OutN = 0;
	while(x)
	{
		Out[OutN++] = x%10+'0';
		x /= 10;
	}
	while(OutN--)
		putchar(Out[OutN]);
	puts("");
}

const int N = 100009;
const int M = 50009;

int n, m;
int a[N];
int pos[N];
int idQ[N];
long long total;
int cnt[N];
pair<int,int> q[M], st[M];
long long ans[M], add[M];

namespace TreeArray
{
	int sum[N];
	#define lowbit(x) ((x)&-(x))
	inline void init()
	{
		memset(sum, 0, sizeof sum);
	}
	inline int query(int x)
	{
		int ret = 0;
		for(int i = x; i; i -= lowbit(i))
			ret += sum[i];
		return ret;
	}
	inline int query(int l, int r)
	{
		return query(r)-query(l-1);
	}
	inline void modify(int x, int v)
	{
		for(int i = x; i <= n; i += lowbit(i))
			sum[i] += v;
	}
}

inline void init()
{
	read(n), read(m);
	for(int i = 1; i <= n; ++i)
		read(a[i]), pos[a[i]] = i;
	for(int i = 1; i <= m; ++i)
	{
		read(q[i].second), q[i].first = pos[q[i].second];
		idQ[q[i].second] = i;
	}
}

void divide_and_conquer(int l, int r)
{
	if(l == r) return ;
	int mid = (l+r)>>1;
	divide_and_conquer(l, mid);
	divide_and_conquer(mid+1, r);
	int top = 0;
	for(int i = l, j = mid+1; i <= mid || j <= r;)
		if(i <= mid && (j == r+1 || q[i] < q[j])) st[++top] = q[i++];
		else st[++top] = q[j++];
	for(int i = 1; i <= top; ++i)
		q[l+i-1] = st[i];
	for(int i = 1; i <= top; ++i)
		if(idQ[st[i].second] <= mid)
			TreeArray::modify(st[i].second, 1);
		else add[idQ[st[i].second]] += TreeArray::query(st[i].second+1, n);
	for(int i = 1; i <= top; ++i)
		if(idQ[st[i].second] <= mid)
			TreeArray::modify(st[i].second, -1);
	for(int i = top; i >= 1; --i)
		if(idQ[st[i].second] <= mid)
			TreeArray::modify(st[i].second, 1);
		else add[idQ[st[i].second]] += TreeArray::query(st[i].second-1);
	for(int i = 1; i <= top; ++i)
		if(idQ[st[i].second] <= mid)
			TreeArray::modify(st[i].second, -1);
}

inline void solve()
{
	for(int i = n; i >= 1; --i)
	{
		cnt[i] = TreeArray::query(a[i]-1);
		TreeArray::modify(a[i], 1);total += cnt[i];
	}
	TreeArray::init();
	for(int i = 1; i <= n; ++i)
	{
		cnt[i] += TreeArray::query(a[i]+1, n);
		TreeArray::modify(a[i], 1);
		if(idQ[a[i]]) ans[idQ[a[i]]] = -cnt[i];
	}
	TreeArray::init();
	ans[0] = total;
	for(int i = 1; i <= m; ++i)
		ans[i] += ans[i-1];
	divide_and_conquer(1, m);
	long long p = 0;
	for(int i = 1; i <= n; ++i)
	{
		p += add[i];
		ans[i] += p;
	}
	for(int i = 1; i <= m; ++i)
		write(ans[i-1]);
}

int main()
{
	freopen("bzoj3295.in", "r", stdin);
	freopen("bzoj3295.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
