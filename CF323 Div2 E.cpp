#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int MAXN = 2e5+9;
const int INF = 1e9;

int n;
int a[MAXN];
bool ok[MAXN];
vector<int> fac;
vector<int> Max[2000];
vector<int> len[2000];
vector<int> p[2000];
int q[MAXN];
long long ans;

inline int gcd(int a, int b)
{
	return b?gcd(b, a%b):a;	
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("E.in", "r", stdin);
	freopen("E.out", "w", stdout);
#endif

	scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
		scanf("%d", a+i);
	for(int i = 1; i <= n; ++i)
		if(n%i == 0)
		{
			fac.push_back(i);
			q[i] = fac.size()-1;
		}
	for(int i = 0; i < (int)fac.size(); ++i)
	{
		int d = fac[i], p = n/d;
		for(int j = 0; j < d; ++j)
		{
			int val = -INF;	
			for(int k = 0; k < p; ++k)
				val = max(val, a[k*d+j+1]);
			Max[i].push_back(val);
		}
	}
	for(int i = 0; i < (int)fac.size(); ++i)
	{
		int d = fac[i];
		bool all = true;
		memset(ok, 0, sizeof ok);
		for(int j = 1; j <= n; ++j)
			if(a[j] == Max[i][(j-1)%d])
				ok[j] = true;
			else all = false;
		if(all) len[i].push_back(n);
		else
		{
			int l = 0, r = n+1;
			if(ok[1] == ok[n])
			{
				while(ok[l+1]) l++;
				while(ok[r-1]) r--;
				len[i].push_back(l+n-r+1);	
			}
			l++;
			while(l < r)
			{
				int last;
				while(!ok[l] && l < r) l++;last = l;
				if(l >= r) break;
				while(ok[l+1] && l < r) l++;l++;
				len[i].push_back(l-last);
			}
		}
	}
	for(int i = 1; i < n; ++i)
		p[q[gcd(i, n)]].push_back(i/gcd(i, n));
	for(int i = 0; i < (int)fac.size(); ++i)
	{
		int last = 0;long long sum = 0;
		sort(len[i].begin(), len[i].end());
		for(int j = 0; j < (int)len[i].size(); ++j)
			sum += len[i][j];
		for(int j = 0; j < (int)p[i].size(); ++j)
		{
			if(len[i].size() == 1 && sum == n)
			{
				ans += n;
				continue;
			}
			while(last < (int)len[i].size() && len[i][last]-p[i][j]*fac[i]+1 <= 0) 
			{
				sum -= len[i][last];
				last++;
			}
			ans += sum-p[i][j]*fac[i]*(len[i].size()-last)+(len[i].size()-last);
		}
	}
	cout << ans << endl;
	
	return 0;
}
