#include <cstdio>
#include <cstring>
#include <algorithm>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
#define clr(a, b) memset(a, b, sizeof a)
using namespace std;

const int MAXN = 509;
const int MAXM = 5009;
const int INF = 0x3f3f3f3f;

struct stuff
{
	int p, q, v;
	inline void read()
	{
		scanf("%d%d%d", &p, &q, &v);
	}
};

int n, m;
stuff a[MAXN];
int f[MAXM];

inline bool cmp(const stuff &a, const stuff &b)
{
	return a.q-a.p > b.q-b.p;
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	#endif
	
	while(~scanf("%d%d", &n, &m))
	{
		rep(i, 1, n)
			a[i].read();
		sort(a+1, a+n+1, cmp);
		clr(f, -INF);
		f[0] = 0;
		rep(i, 1, n)
			per(j, m, 0)
				if(m-j+a[i].p >= a[i].q && j >= a[i].p)
					f[j] = max(f[j], f[j-a[i].p]+a[i].v);
		int ans = 0;
		rep(i, 0, m)
			ans = max(ans, f[i]);
		printf("%d\n", ans);
	}
	
	return 0;
}
