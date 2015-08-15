#include <cstdio>
#include <cstring>
#include <algorithm>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
#define clr(a, b) memset(a, b, sizeof a)
using namespace std;

template<class T> void read(T &x)
{
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}
template<class T> void prt(T x)
{
    if(!x) putchar('0');
    else
    {
    	if(x < 0)
    	{
    		putchar('-');
    		x = -x;
		}
        int tot = 0;
        char s[20] = "\0";
        while(x)
        {
            s[++tot] = x%10+'0';
            x /= 10;
        }
        per(i, tot, 1)
            putchar(s[i]);
    }
}

const int MAXN = 50009;
const int MAXM = 10009;
const int SIZE = 2500000;

struct Query
{
	int type;
	int a, b, c;
};

int n, m;
int a[MAXN];
int val[MAXN+MAXM], tot;
Query q[MAXM];

int root[(MAXN+MAXM)<<2], sz;
int ls[SIZE], rs[SIZE], sum[SIZE];

void build(int x, int l, int r)
{
	root[x] = 0;
	if(l == r) return ;
	int mid = (l+r)>>1;
	build(x<<1, l, mid);
	build(x<<1|1, mid+1, r);
}

void ins2(int &x, int l, int r, int p, int q)
{
	if(!x)
	{
		x = ++sz;
		ls[x] = rs[x] = sum[x] = 0;
	}
	if(l == r)
	{
		sum[x] += q;
		return ;
	}
	int mid = (l+r)>>1;
	if(p <= mid) ins2(ls[x], l, mid, p, q);
	else ins2(rs[x], mid+1, r, p, q);
	sum[x] = sum[ls[x]]+sum[rs[x]];
}

void ins(int x, int l, int r, int val, int p, int q)
{
	ins2(root[x], 1, n, p, q);
	if(l == r) return ;
	int mid = (l+r)>>1;
	if(val <= mid) ins(x<<1, l, mid, val, p, q);
	else ins(x<<1|1, mid+1, r, val, p, q);
}

int qry2(int x, int l, int r, int ll, int rr)
{
	if(!x) return 0;
	if(l == ll && r == rr) return sum[x];
	int mid = (l+r)>>1;
	if(rr <= mid) return qry2(ls[x], l, mid, ll, rr);
	else if(ll > mid) return qry2(rs[x], mid+1, r, ll, rr);
	else return qry2(ls[x], l, mid, ll, mid)+qry2(rs[x], mid+1, r, mid+1, rr);
}

int qry(int ll, int rr, int k)
{
	int x = 1, l = 1, r = tot;
	while(l < r)
	{
		int mid = (l+r)>>1;
		int tmp = qry2(root[x<<1], 1, n, ll, rr);
		if(tmp >= k) x = x<<1, r = mid;
		else k -= tmp, x = x<<1|1, l = mid+1;
	}
	return l;
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	#endif
	
	int Case;
	read(Case);
	while(Case--)
	{
		read(n), read(m);
		tot = 0;
		rep(i, 1, n)
		{
			read(a[i]);
			val[++tot] = a[i];
		}
		rep(i, 1, m)
		{
			char s[3] = "\0";
			scanf("%s", s);
			if(s[0] == 'Q')
			{
				q[i].type = 1;
				read(q[i].a), read(q[i].b), read(q[i].c);
			}
			else
			{
				q[i].type = 2;
				read(q[i].a), read(q[i].b);
				val[++tot] = q[i].b;
			}
		}
		sort(val+1, val+tot+1);
		sz = 0;build(1, 1, tot);
		rep(i, 1, n)
			ins(1, 1, tot, lower_bound(val+1, val+tot+1, a[i])-val, i, 1);
		rep(i, 1, m)
			if(q[i].type == 1)
				prt(val[qry(q[i].a, q[i].b, q[i].c)]), puts("");
			else 
			{
				ins(1, 1, tot, lower_bound(val+1, val+tot+1, a[q[i].a])-val, q[i].a, -1);
				ins(1, 1, tot, lower_bound(val+1, val+tot+1, a[q[i].a] = q[i].b)-val, q[i].a, 1);
			}
	}
	
	return 0;
}
