#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 79;
const int INF = 0x3f3f3f3f;

int n, k;
struct Data
{
	int data, key, fre;
	Data() {data = key = fre = 0;}
}a[N];
int val[N], valN;
int f[N][N][N];
int s[N][N];

void init()
{
	cin >> n >> k;
	for(int i = 1; i <= n; ++i)
		cin >> a[i].data;
	for(int i = 1; i <= n; ++i)
	{
		cin >> a[i].key;
		val[++valN] = a[i].key;
	}
	for(int i = 1; i <= n; ++i)
		cin >> a[i].fre;
}

inline bool cmp(const Data &p, const Data &q)
{
	return p.data < q.data;
}

void solve()
{
	sort(a+1, a+n+1, cmp);
	sort(val+1, val+valN+1);
	valN = unique(val+1, val+valN+1)-val-1;
	for(int i = 1; i <= n; ++i)
		a[i].key = lower_bound(val+1, val+valN+1, a[i].key)-val;
	for(int i = 1; i <= n; ++i)
		for(int j = i; j <= n; ++j)
			s[i][j] = s[i][j-1]+a[j].fre;
	for(int i = 1; i <= n; ++i)
		for(int j = i; j <= n; ++j)
			memset(f[i][j], INF, sizeof f[i][j]);
	for(int i = 1; i <= n; ++i)
		for(int j = 0; j <= a[i].key; ++j)
			f[i][i][j] = a[i].fre;
	for(int l = 1; l <= n; ++l)
		for(int i = 1; i+l-1 <= n; ++i)
		{
			int j = i+l-1;
			for(int p = valN; p >= 0; --p)
			{
				int tmp = f[i][j][p+1];
				for(int q = i; q <= j; ++q)
				{
					if(a[q].key >= p)
						tmp = min(tmp, f[i][q-1][a[q].key]+f[q+1][j][a[q].key]+s[i][j]);
					tmp = min(tmp, f[i][q-1][p]+f[q+1][j][p]+s[i][j]+k);
				}
				f[i][j][p] = tmp;
			}
		}
	cout << f[1][n][0] << endl;
}

int main()
{
	freopen("treapmod.in", "r", stdin);
	freopen("treapmod.out", "w", stdout);
	
	init();
	solve();
	
	fclose(stdin);fclose(stdout);
	return 0;
}
