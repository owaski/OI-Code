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

const int MAXN = 10009;
const int SIGMA = 26;

struct Node
{
	int l;
	Node *p, *ne[SIGMA];
	inline void init(int len)
	{
		l = len, p = NULL;
		memset(ne, 0, sizeof ne);
	}
}pool[MAXN<<1], *cur, *root, *last;

inline void init()
{
	cur = root = last = pool, last->init(0);
}

inline Node* newnode(int len)
{
	Node *p = ++cur;
	p->init(len);
	return p;
}

char s[MAXN<<1];

inline void add(int c)
{
	Node *p = last;
	Node *np = newnode(p->l+1);
	while(p && !p->ne[c])
		p->ne[c] = np, p = p->p;
	if(!p) np->p = root;
	else
	{
		Node *q = p->ne[c];
		if(p->l+1 == q->l) np->p = q;
		else
		{
			Node *r = ++cur;
			r[0] = q[0], r->l = p->l+1;
			q->p = np->p = r;
			while(p && p->ne[c] == q)
				p->ne[c] = r, p = p->p;
		}
	}
	last = np;
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("beads.in", "r", stdin);
	freopen("beads.out", "w", stdout);
	#endif

	int Case;
	read(Case);
	while(Case--)
	{
		init();
		scanf("%s", s+1);
		int n = strlen(s+1);
		rep(i, 1, n)
			s[i+n] = s[i];
		rep(i, 1, n<<1)
			add(s[i]-'a');
		Node *now = root;
		rep(i, 1, n)
			rep(j, 0, SIGMA-1)
				if(now->ne[j])
				{
					now = now->ne[j];
					break;
				}
		prt(now->l-n+1);
		puts("");
	}

	return 0;
}
