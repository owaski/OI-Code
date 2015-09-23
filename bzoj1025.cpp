#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
typedef long long LL;
using namespace std;

const int MAXN = 1009;

int prime[MAXN], tot;
bool is[MAXN];

int n;
LL f[MAXN][MAXN];

inline void euler(int lim)
{
    rep(i, 2, lim)
    {
        if(!is[i])
        	prime[++tot] = i;
        for(int j = 1; j <= tot && prime[j]*i <= lim; ++j)
        {
            is[i*prime[j]] = true;
            if(i%prime[j] == 0)
                break;
        }
    }
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1025.in", "r", stdin);
	freopen("bzoj1025.out", "w", stdout);
#endif

    scanf("%d", &n);
    euler(n);
    f[0][0] = 1;
	rep(i, 1, n)
		f[i][0] = 1;
	rep(i, 1, tot)
		f[0][i] = 1;
	rep(i, 1, n)
		rep(j, 1, tot)
		{
			f[i][j] = f[i][j-1];
			for(int k = prime[j]; k <= i; k *= prime[j])
				f[i][j] += f[i-k][j-1]; 
		}
	printf("%lld\n", f[n][tot]);

	return 0;
}
