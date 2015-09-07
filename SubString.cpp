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

const int SIZE = 1200009;
const int SIGMA = 26;

struct Lct *null;
struct Lct
{
	Lct *fa, *c[2];
	int rig, add;
	inline void init(int v)
	{
		fa = c[0] = c[1] = null;
		rig = v, add = 0;
	}
	inline void setc(Lct *p, int d)
	{
		c[d] = p, p->fa = this;
	}
	inline bool d()
	{
		return fa->c[1] == this;
	}
	inline bool is_root()
	{
		return fa == null || fa->c[0] != this && fa->c[1] != this;
	}
	inline void upd_add(int v)
	{
		rig += v;
		add += v;
	}
	inline void push_down()
	{
		if(add)
		{
			rep(i, 0, 1)
				c[i]->upd_add(add);
			add = 0;
		}
	}
	inline void rot()
	{
		Lct *f = fa, *ff = f->fa;
		int p = d(), pp = f->d();
		if(f->is_root()) fa = ff;
		else ff->setc(this, pp);
		f->setc(c[p^1], p), setc(f, p^1);
	}
	inline void splay()
	{
		int n = 0;
		static Lct *anc[SIZE];
		anc[++n] = this;
		for(Lct *p = this; !p->is_root(); p = p->fa)
			anc[++n] = p->fa;
		per(i, n, 1) anc[i]->push_down();
		while(!is_root())
		{
			Lct *f = fa;
			if(!f->is_root())
				if(d() == f->d()) f->rot();
				else rot();
			rot();
		}
	}
	inline void access()
	{
		for(Lct *p = this, *q = null; p != null; q = p, p = p->fa)
			p->splay(), p->setc(q, 1);
	}
}p1[SIZE], *c1 = p1;

struct Sam
{
	Sam *fa, *ne[SIGMA];
	int l;Lct *p;
	inline void init(int len, int val)
	{
		l = len, fa = 0;
		memset(ne, 0, sizeof ne);
		p = ++c1, p->init(val);
	}
}p2[SIZE], *c2 = p2, *root, *last;

inline void init()
{
	null = c1, null->init(0);
	root = last = c2, root->init(0, 0);
}

inline Sam* newnode(int len, int val)
{
	Sam *p = ++c2;
	p->init(len, val);
	return p;
}

inline void cut(Lct *a)
{
	a->access(), a->splay(), a->c[0]->upd_add(-a->rig);
	a->c[0] = a->c[0]->fa = null;
}

inline void link(Lct *a, Lct *b)
{
	a->access(), a->splay(), a->upd_add(b->rig);
	b->fa = a;
}

inline void ins(int x)
{
	Sam *p = last;
	Sam *np = newnode(p->l+1, 1);
	while(p && !p->ne[x])
		p->ne[x] = np, p = p->fa;
	if(!p) np->fa = root, link(root->p, np->p);
	else
	{
		Sam *q = p->ne[x];
		if(q->l == p->l+1) np->fa = q, link(q->p, np->p);
		else
		{
			Sam *r = newnode(p->l+1, 0);
			memcpy(r->ne, q->ne, sizeof r->ne);
			link(q->fa->p, r->p);
			cut(q->p), link(r->p, q->p);
			link(r->p, np->p);
			r->fa = q->fa, q->fa = np->fa = r;
			while(p && p->ne[x] == q)
				p->ne[x] = r, p = p->fa;
		}
	}
	last = np;
}

int q;
char s[SIZE];
int mask;

inline void decode(char *s, int mask)
{
	int len = strlen(s);
	rep(i, 0, len-1)
	{
		mask = (mask*131+i)%len;
		swap(s[i], s[mask]);
	}
}

inline int solve(char *s)
{
	Sam *now = root;
	while(*s)
	{
		int id = (*s++)-'A';
		if(!now->ne[id])
			return 0;
		now = now->ne[id];
	}
	now->p->splay();
	return now->p->rig;
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("SubString.in", "r", stdin);
	freopen("SubString.out", "w", stdout);
	#endif

	read(q);
	init();
	scanf("%s", s);
	int len = strlen(s);
	rep(i, 0, len-1)
		ins(s[i]-'A');
	while(q--)
	{
		char opt[10] = "\0";
		scanf("%s%s", opt, s);
		decode(s, mask);
		if(opt[0] == 'A')
		{
			int len = strlen(s);
			rep(i, 0, len-1)
				ins(s[i]-'A');
		}
		else
		{
			int result = solve(s);
			prt(result), puts("");
			mask ^= result;
		}
	}
	return 0;
}
 
