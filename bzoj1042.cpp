#include <bits/stdc++.h>
using namespace std;

const int MAXS = 100009;

int c[5], tot;
int d[5], s;
long long f[MAXS], ans;

inline long long get(int a, int b, int p, int q)
{
	int sum = a*c[1]+b*c[2]+p*c[3]+q*c[4];
	if(sum > s) return 0;
	else return f[s-sum];
}

int main()
{
	freopen("bzoj1042.in", "r", stdin);
	freopen("bzoj1042.out", "w", stdout);
	
	for(int i = 1; i <= 4; ++i)
		scanf("%d", c+i);
	f[0] = 1;
	for(int i = 1; i <= 4; ++i)
		for(int j = c[i]; j <= 100000; ++j)
			f[j] += f[j-c[i]];
	scanf("%d", &tot);
	while(tot--)
	{
		for(int i = 1; i <= 4; ++i)
			scanf("%d", d+i);
		scanf("%d", &s);ans = 0;
		for(int i = 0; i < 16; ++i)
		{
			int p[5] = {0}, g = 1;
			for(int j = 0; j < 4; ++j)
				if(i&(1<<j))
				{
					p[j+1] = 1;
					g = -g;	
				}
			ans += g*get(p[1]*(d[1]+1), p[2]*(d[2]+1), p[3]*(d[3]+1), p[4]*(d[4]+1));
		}
		cout << ans << endl;
	}
	
	return 0;
}
