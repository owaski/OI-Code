#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int MAXN = 509;

int n;
map<int,int> s;
int ans[MAXN];

int gcd(int a, int b)
{
	return b?gcd(b, a%b):a;
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("A.in", "r", stdin);
	freopen("A.out", "w", stdout);
#endif

	scanf("%d", &n);
	for(int i = 1; i <= n*n; ++i)
	{
		int x;
		scanf("%d", &x);
		s[x]++;
	}
	for(int i = 1; i <= n; ++i)
	{
		map<int,int>::iterator it = s.end();
		--it, ans[i] = it->first, s[ans[i]]--;
		if(s[ans[i]] == 0) s.erase(ans[i]);
		for(int j = 1; j < i; ++j)
		{
			int d = gcd(ans[i], ans[j]);
			s[d] -= 2;if(s[d] == 0)	s.erase(d);
		}
	}
	for(int i = 1; i <= n; ++i)
		printf("%d%c", ans[i], i==n?'\n':' ');

	return 0;	
}
