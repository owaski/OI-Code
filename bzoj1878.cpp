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

const int N = 50009;
const int M = 200009;
const int V = 1000009;

int n, m;
int a[N];
int prePos[N];
int pos[V];
struct Query
{
	int l, r, id;
	inline void input()
	{
		read(l), read(r);
	}
	inline friend bool operator < (const Query &a, const Query &b)
	{
		return a.r < b.r;
	}
}q[M];
int ans[M];
int sum[N];

inline void init()
{
	read(n);
	for(int i = 1; i <= n; ++i)
	{
		read(a[i]);
		prePos[i] = pos[a[i]];
		pos[a[i]] = i;
	}
	read(m);
	for(int i = 1; i <= m; ++i)
	{
		q[i].input();
		q[i].id = i;
	}		
}

#define lowbit(x) ((x)&-(x))

inline void insert(int x, int v)
{
	for(int i = x; i <= n; i += lowbit(i))
		sum[i] += v;
}

inline int query(int x)
{
	int ret = 0;
	for(int i = x; i; i -= lowbit(i))
		ret += sum[i];
	return ret;
}

inline void solve()
{
	int pt = 0;
	sort(q+1, q+m+1);
	for(int i = 1; i <= m; ++i)
	{
		while(pt+1 <= q[i].r)
		{
			++pt;
			if(prePos[pt])
				insert(prePos[pt], -1);
			insert(pt, 1);
		}
		ans[q[i].id] = query(q[i].r)-query(q[i].l-1);
	}
	for(int i = 1; i <= m; ++i)
		write(ans[i]), putchar('\n');
}

int main()
{
	freopen("bzoj1878.in", "r", stdin);
	freopen("bzoj1878.out", "w", stdout);

	init();
	solve();

	fclose(stdin);fclose(stdout);
	return 0;
}
