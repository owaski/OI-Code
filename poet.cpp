#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 100009;
const long double INF = (long double)1000000000000000001LL;

int n, L, P;
char str[N][32];
int sum[N];
long double f[N];
int from[N];
struct Data
{
	int l, r, id;
	Data() {l = r = id = 0;}
	Data(int _l, int _r, int _id):l(_l), r(_r), id(_id) {}
}q[N];

void init()
{
	cin >> n >> L >> P;L++;
	for(int i = 1; i <= n; ++i)
	{
		scanf("%s", str[i]);
		sum[i] = strlen(str[i])+sum[i-1]+1;
		f[i] = INF;
	}
}

void printStep(int cur)
{
	if(cur) printStep(from[cur]);
	for(int i = from[cur]+1; i <= cur; ++i)
		printf("%s%c", str[i], i==cur?'\n':' ');
}

long double power(long double a)
{
	long double ret = 1;
	for(int i = 1; i <= P; ++i)
		ret *= a;
	return ret;
}

long double w(int j, int i)
{
	return power(abs(sum[i]-sum[j]-L));
}

long double t(int j, int i)
{
	return f[j]+w(j, i);
}

int binarySearch(const Data &p, int i)
{
	int l = p.l, r = p.r+1;
	while(l < r)
	{
		int mid = (l+r)>>1;
		if(t(p.id, mid) >= t(i, mid)) r = mid;
		else l = mid+1;
	}
	return l;
}

void solve()
{
	int head, tail;
	head = tail = 0;
	q[0] = Data(0, n, 0);
	for(int i = 1; i <= n; ++i)
	{
		while(head < tail && q[head].r < i) ++head;
		f[i] = t(q[head].id, i);from[i] = q[head].id;
		while(head <= tail && t(i, q[tail].l) <= t(q[tail].id, q[tail].l)) --tail;
		if(head > tail) q[++tail] = Data(i, n, i);
		else
		{
			int g = binarySearch(q[tail], i);
			q[tail].r = g-1;
			if(g <= n) q[++tail] = Data(g, n, i);
		}
		//debug("%lld\n", f[i]);
	}
	//debug("\n");
	if(f[n] >= INF)
		puts("Too hard to arrange");
	else
	{
		cout << (long long)f[n] << endl;
		printStep(n);
	}
	puts("--------------------");
}

int main()
{
	freopen("poet.in", "r", stdin);
	freopen("poet.out", "w", stdout);
	
	int Case;
	cin >> Case;
	while(Case--)
	{
		init();
		solve();
	}
	
	fclose(stdin);fclose(stdout);
	return 0;
}
