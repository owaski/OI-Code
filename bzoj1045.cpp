#include <bits/stdc++.h>
using namespace std;

template<class T>
inline void read(T &x)
{
	char c;x = 0;
	while((c=getchar()) < '0' || c > '9');
	x = c-'0';
	while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
}

const int MAXN = 1000009;

int n;
long long avg;
long long a[MAXN];
long long x[MAXN];
long long add;
long long ans;

int main()
{
	freopen("bzoj1045.in", "r", stdin);
	freopen("bzoj1045.out", "w", stdout);
	
	read(n);
	for(int i = 1; i <= n; ++i)
	{
		read(a[i]);
		avg += a[i];	
	}
	avg /= n;
	x[1] = 0;
	for(int i = 1; i < n; ++i)
		x[i+1] = x[i]-avg+a[i];
	int p = (n+1)>>1;
	nth_element(x+1, x+p-1, x+n+1);
	nth_element(x+p, x+p, x+n+1);
	add = -x[p];
	for(int i = 1; i <= n; ++i)
	{
		x[i] += add;
		ans += abs(x[i]);
	}
	cout << ans << endl;
	
	return 0;
}
