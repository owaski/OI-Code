#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
using namespace std;

const int MAXN = 100009;

int n;
int a[MAXN];
int b[MAXN];

inline int solve(int *a, int *b)
{
	int ret = 0;
	int l1 = 1, r1 = n, l2 = 1, r2 = n;
	while(l1 <= r1 && l2 <= r2)
	{
		if(a[l1] > b[l2])
			ret += 2, l1++, l2++;
		else if(a[r1] > b[r2])
			ret += 2, r1--, r2--;
		else
		{
			ret += (a[l1]==b[r2]);
			l1++, r2--;
		}
	}
	return ret;
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1034.in", "r", stdin);
	freopen("bzoj1034.out", "w", stdout);
#endif

	scanf("%d", &n);
	rep(i, 1, n) scanf("%d", a+i);
	rep(i, 1, n) scanf("%d", b+i);
	sort(a+1, a+n+1), sort(b+1, b+n+1);
	printf("%d %d\n", solve(a, b), n*2-solve(b, a));

	return 0;
}
