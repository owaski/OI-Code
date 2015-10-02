#include <bits/stdc++.h>
using namespace std;

const int MAXN = 50009;
const int MAXM = 1009;
const int MOD = 10007;

int n, m;
int l[MAXN];
int f[2][MAXN];
int ans;

inline bool check(int maxl)
{
	int block = 1, cnt = 0;
	for(int i = 1; i <= n; ++i)
	{
		cnt += l[i];
		if(cnt > maxl)
		{
			block++;
			cnt = l[i];	
		}
	}
	return block-1 <= m;
}

template<class T>
inline void mod(T &x)
{
	while(x >= MOD) x -= MOD;	
	while(x < 0) x += MOD;
}

int main()
{
	freopen("bzoj1044.in", "r", stdin);
	freopen("bzoj1044.out", "w", stdout);
	
	scanf("%d%d", &n, &m);
	int le = 0, ri = 0;
	for(int i = 1; i <= n; ++i)
	{
		scanf("%d", l+i);
		ri += l[i], le = max(le, l[i]);
	}
	while(le < ri)
	{
		int mid = (le+ri)>>1;
		if(check(mid)) ri = mid;
		else le = mid+1;	
	}
	f[0][0] = 1;
	for(int j = 1; j <= m+1; ++j)
	{
		int now = j&1;
		int len = 0, last = 0;
		int cnt = 0;
		for(int i = 1; i <= n; ++i)
		{
			len += l[i];
			mod(cnt += f[now^1][i-1]);
			while(len > le)
			{
				mod(cnt -= f[now^1][last]);
				len -= l[++last];
			}
			f[now][i] = cnt;
		}
		mod(ans += f[now][n]);
		for(int i = 0; i <= n; ++i)
			f[now^1][i] = 0;
	}
	printf("%d %d\n", le, ans);
	
	return 0;	
}
