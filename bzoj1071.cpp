#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 5009;

int n, A, B, C;
struct Data
{
	int h, s, sum;
	void input()
	{
		scanf("%d%d", &h, &s);
		sum = h*A+s*B;
	}
}a[N];
int h[N];
int sum[N];
int in[N];
int ans;

void init()
{
	scanf("%d%d%d%d", &n, &A, &B, &C);
	for(int i = 1; i <= n; ++i)
		a[i].input(), h[i] = sum[i] = i;
}

bool cmph(int p, int q)
{
	return a[p].h < a[q].h;
}

bool cmpsum(int p, int q)
{
	return a[p].sum < a[q].sum;
}

void solve()
{
	sort(h+1, h+n+1, cmph);
	sort(sum+1, sum+n+1, cmpsum);
	for(int i = 1; i <= n; ++i)
	{
		int mins = a[i].s;
		int l = 0, r = 0, cnt = 0;
		memset(in, false, sizeof in);
		for(int j = 1; j <= n; ++j)
		{
			while(r < n && a[sum[r+1]].sum <= C+mins*B+a[h[j]].h*A)
				++r, cnt += (in[sum[r]] = a[sum[r]].h >= a[h[j]].h && a[sum[r]].s >= mins);
			while(l < n && a[h[l+1]].h < a[h[j]].h)
				++l, cnt -= in[h[l]];
			ans = max(ans, cnt);
		}
	}
	cout << ans << endl;
}

int main()
{
	freopen("bzoj1071.in", "r", stdin);
	freopen("bzoj1071.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
