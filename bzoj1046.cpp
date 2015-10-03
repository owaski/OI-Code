#include <bits/stdc++.h>
using namespace std;

const int MAXN = 10009;

int n, m;
int a[MAXN];
int maxl[MAXN];
int Max[MAXN];
int len;

inline void get()
{
	len = 1;
	Max[1] = -a[n], maxl[n] = 1;
	for(int i = n-1; i >= 1; --i)
	{
		int p = lower_bound(Max+1, Max+len+1, -a[i])-Max;
		maxl[i] = p, Max[p] = -a[i], len = max(len, p);
	}
}

int main()
{
	freopen("bzoj1046.in", "r", stdin);
	freopen("bzoj1046.out", "w", stdout);
	
	scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
		scanf("%d", a+i);
	get(), scanf("%d", &m);
	while(m--)
	{
		int l;
		scanf("%d", &l);
		if(l > len) puts("Impossible");
		else
		{
			int id = 1, last = 0;
			while(id <= n && l)
			{
				if(maxl[id] >= l && a[id] > last)
				{
					printf("%d", a[id]);
					l--, last = a[id];
					if(!l) puts("");
					else putchar(' ');
				}
				id++;
			}
		}
	}
	
	return 0;
}
