#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

template<class T>
inline void read(T &x)
{
	char c;int f = 1;x = 0;
	while(((c=getchar()) < '0' || c > '9') && c != '-');
	if(c == '-') f = -1;else x = c-'0';
	while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
	x *= f;
}

int OutN;
char Out[20];

template<class T>
inline void write(T x)
{
	if(x < 0) putchar('-'), x = -x;
	else if(!x) {puts("0");return ;}
	OutN = 0;
	while(x)
	{
		Out[++OutN] = x%10+'0';
		x /= 10;
	}
	while(OutN--)
		putchar(Out[OutN+1]);
	puts("");
}

const int N = 2009;
const int M = 4009;
const int C = 1000009;

int len, Case;
int n, m;
int s[2][N][M];
int x[C], y[C];

#define lowbit(x) ((x)&(-x))

inline void modify(int f, int x, int y, int v)
{
	for(++x, ++y; x <= n+1; x += lowbit(x))
		for(int i = y; i <= m+1; i += lowbit(i))
			s[f][x][i] += v;
}

inline void insert(int t, int c, int l, int r, int d)
{
	x[c] = (t-d*l+n)%n, y[c] = r-l;
	modify(0, x[c], y[c]+x[c], 1), modify(1, x[c], y[c]-x[c]+n, 1);
}

inline int getSum(int f, int x, int y)
{
	if(x < 0 || y < 0) return 0;
	if(++x > n) x = n+1;if(++y > m) y = m+1;
	int ret = 0;
	for(; x; x -= lowbit(x))
		for(int i = y; i; i -= lowbit(i))
			ret += s[f][x][i];
	return ret;
}

inline int sum(int f, int x1, int y1, int x2, int y2)
{
	return getSum(f, x2, y2)+getSum(f, x1-1, y1-1)-getSum(f, x1-1, y2)-getSum(f, x2, y1-1);
}

inline int query(int t, int l, int r)
{
	int d = r == len;
	return sum(0, t, l+t, t+r, m)+sum(0, 0, l+t-n, t+r-n-d, m)+sum(1, t-r, l-t+n, t-1, m)+sum(1, n+t-r+d, l-t, n, m);
}

inline void del(int c)
{
	modify(0, x[c], y[c]+x[c], -1), modify(1, x[c], y[c]-x[c]+n, -1);
}

int main()
{
	freopen("bzoj1062.in", "r", stdin);
	freopen("bzoj1062.out", "w", stdout);
	
	read(Case), read(len);
	n = len<<1, m = len<<2;
	while(Case--)
	{
		int tp;read(tp);
		int t, l, r, c, d;
		read(t);
		if(tp == 1)
		{
			read(c), read(l), read(r), read(d);
			insert(t, c, l, r, d);
		}
		else if(tp == 2)
		{
			read(l), read(r);
			write(query(t%n, l, r));
		}
		else read(c), del(c);
	}
	
	fclose(stdin);fclose(stdout);
	return 0;
}
