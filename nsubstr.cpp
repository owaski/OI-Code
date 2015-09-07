#include <bits/stdc++.h>
#define pii pair<int,int>
#define X first
#define Y second
#define pb push_back
#define mp make_pair
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
#define debug(...) fprintf(stderr, __VA_ARGS__)
typedef unsigned long long ULL;
typedef long long LL;
typedef double DB;
typedef long double LD;
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

const int MAXN = 250009;
const int SIGMA = 26;

struct Node
{
	int r, l;
	bool vis;
	Node *f, *ne[SIGMA];
	inline void init(int len)
	{
		vis = false, r = 0, l = len;
		f = 0, memset(ne, 0, sizeof ne);
	}
}pool[MAXN<<1], *cur;
Node *last, *root;

inline void init()
{
	cur = last = root = pool, cur->init(0);
}

inline Node* newnode(int len)
{
	 Node *p = ++cur;
	 p->init(len);
	 return p;
}

inline void add(int x)
{
	Node *p = last;
	Node *np = newnode(p->l+1);
	np->r = 1;
	while(p && !p->ne[x])
		p->ne[x] = np, p = p->f;
	if(!p) np->f = root;
	else
	{
		Node *q = p->ne[x];
		if(p->l+1 == q->l) np->f = q;
		else
		{
			Node *r = newnode(p->l+1);
			memcpy(r->ne, q->ne, sizeof q->ne);
			r->f = q->f, np->f = q->f = r;
			while(p && p->ne[x] == q)
				p->ne[x] = r, p = p->f;
		}
	}
	last = np;
}

int n;
int f[MAXN];
char s[MAXN];
int cnt[MAXN];
Node *b[MAXN<<1];

inline void solve()
{
	int sz = cur-pool;
	int Max = 0;
	rep(i, 1, sz)
		Max = max(Max, pool[i].l), cnt[pool[i].l]++;
	rep(i, 1, Max)
		cnt[i] += cnt[i-1];
	rep(i, 1, sz)
		b[cnt[pool[i].l]--] = &pool[i];
	per(i, sz, 1)
	{
		Node *p = b[i];
		p->f->r += p->r;
	}
	rep(i, 1, sz)
		f[b[i]->l] = max(f[b[i]->l], b[i]->r);
	per(i, n, 1)
		f[i] = max(f[i], f[i+1]);
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("nsubstr.in", "r", stdin);
	freopen("nsubstr.out", "w", stdout);
	#endif

	init();
	scanf("%s", s+1);
	n = strlen(s+1);
	rep(i, 1, n)
		add(s[i]-'a');
	solve();
	rep(i, 1, n)
		prt(f[i]), puts("");

	return 0;
}
