#include <bits/stdc++.h>
using namespace std;

unsigned n;
int ans;

inline unsigned gcd(unsigned a, unsigned b)
{
	while(b) b ^= a ^= b ^= a %= b;
	return a;
}

inline int calc(unsigned n)
{
	int ret = 0;
	for(unsigned i = 1; i*i*2 < n; ++i)
	{
		int j = sqrt(n-i*i+0.5);
		if(i*i+j*j == n && gcd(i, j) == 1)
			ret++;
	}
	return ret;
}

int main()
{
	freopen("bzoj1041.in", "r", stdin);
	freopen("bzoj1041.out", "w", stdout);
	
	cin >> n;
	n <<= 1;
	for(unsigned i = 1; i*i <= n; ++i)
		if(n%i == 0)
		{
			ans += calc(n/i);
			if(i*i != n) ans += calc(i);
		}
	ans *= 4, ans += 4;
	cout << ans << endl;
	
	return 0;
}
