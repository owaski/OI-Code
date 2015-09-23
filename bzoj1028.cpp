#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
using namespace std;

const int MAXN = 409;
const int MAXM = 1009;

int n, m;
int sum[MAXN];
int tmp[MAXN];
int ans[MAXN], tot;

inline bool solve()
{
	memcpy(tmp, sum, sizeof tmp);
	rep(i, 1, n)
	{
		tmp[i] %= 3;
		while(tmp[i] && i <= n-2 && tmp[i+1] && tmp[i+2])
			tmp[i]--, tmp[i+1]--, tmp[i+2]--;
		if(tmp[i] != 0)
			return false;
	}
	return true;
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1028.in", "r", stdin);
	freopen("bzoj1028.out", "w", stdout);
#endif

	scanf("%d%d", &n, &m);
	rep(i, 1, 3*m+1)
	{
		int x;
		scanf("%d", &x);
		sum[x]++;
	}
	rep(i, 1, n)
	{
		sum[i]++;
		rep(j, 1, n)
			if(sum[j] >= 2)
			{
				sum[j] -= 2;
				if(solve())
				{
					ans[++tot] = i;
					sum[j] += 2;
					break;
				}
				sum[j] += 2;
			}
		sum[i]--;
	}
	if(!tot)
		puts("NO");
	else
	{
		rep(i, 1, tot-1)
			printf("%d ", ans[i]);
		printf("%d\n", ans[tot]);
	}

	return 0;
}
