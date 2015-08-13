#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <set>
#include <list>
#include <stack>
#include <queue>
#include <vector>
#define pii pair<int,int>
#define X first
#define Y second
#define pb push_back
#define mp make_pair
#define clr(a, b) memset(a, b, sizeof a)
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

const int MAXN = 100009;
const int INF = ~0U>>2; 

struct Node *null;
struct Node
{
	Node *fa, *c[2];
	int val, minv, add, sz;
	bool rev;
	
	inline void init()
	{
		fa = c[0] = c[1] = null;
		val = minv = INF, sz = add = rev = 0;
	}
	
	inline void setc(Node *p, int d)
	{
		c[d] = p, p->fa = this;
	}
	inline bool d()
	{
		return fa->c[1] == this;
	}
	inline bool is_root()
	{
		return fa == null;
	}
	inline void push_up()
	{
		minv = min(val, min(c[0]->minv, c[1]->minv));
		sz = c[0]->sz+c[1]->sz+1;
	}
	inline void upd_rev()
	{
		if(this == null)
			return ;
		rev ^= true;
	}
	inline void upd_add(int x)
	{
		if(this == null)
			return ;
		add += x, val += x, minv += x;
	}
	inline void push_down()
	{
		if(rev)
		{
			rep(i, 0, 1)
				c[i]->upd_rev();
			swap(c[0], c[1]);
			rev = false;
		}
		if(add)
		{
			rep(i, 0, 1)
				c[i]->upd_add(add);
			add = 0;
		}
	}
	inline void rot(Node *&r)
	{
		Node *f = fa, *ff = f->fa;
		int p = d(), pp = f->d();
		if(f == r) fa = ff, r = this;
		else ff->setc(this, pp);
		f->setc(c[p^1], p), setc(f, p^1);
		f->push_up();
	}
	inline void splay(Node *&r)
	{
		int n = 0;
		static Node *anc[MAXN];
		anc[++n] = this;
		for(Node *p = this; p != r; p = p->fa)
			anc[++n] = p->fa;
		per(i, n, 1) 
			anc[i]->push_down();
		while(this != r)
		{
			Node *f = fa;
			if(f != r)
				if(d() == f->d()) f->rot(r);
				else rot(r);
			rot(r);
		}
		push_up();
	}
};

int n, m;
Node pool[MAXN<<1], *cur;
Node *r;
int size;

Node* init(int val)
{
	Node *tmp;
	tmp = cur++, tmp->init();
	tmp->val = tmp->minv = val;
	tmp->sz = 1;
	return tmp;
}

Node* find(int p)
{
	Node *now = r;
	while(true)
	{
		now->push_down(); //remember!!! 
		int t = now->c[0]->sz;
		if(t >= p) now = now->c[0];
		else if(t+1 == p) return now;
		else p -= t+1, now = now->c[1];
	}
}

void insert(Node *now, int p, int val)
{
	Node *a, *b, *tmp;
	if(size == p)
	{
		a = find(p), a->splay(r);
		tmp = init(val);
		a->setc(tmp, 1), a->push_up();
	}
	else if(!p)
	{
		b = find(p+1), b->splay(r);
		tmp = init(val);
		b->setc(tmp, 0), b->push_up();
	}
	else
	{
		a = find(p), a->splay(r);
		b = find(p+1), b->splay(a->c[1]);
		tmp = init(val);
		b->setc(tmp, 0), b->push_up(), a->push_up();
	}
	tmp->splay(r), size++;
}

Node* get(int l, int r)
{
	Node *p, *q;
	if(l == 1 && r == size)
		return ::r;
	else if(l == 1)
	{
		q = find(r+1), q->splay(::r);
		return q->c[0];
	}
	else if(r == size)
	{
		p = find(l-1), p->splay(::r);
		return p->c[1];
	}
	else 
	{
		p = find(l-1), p->splay(::r);
		q = find(r+1), q->splay(p->c[1]);
		return q->c[0];
	}
}

void add(int l, int r, int val)
{
	Node *a = get(l, r);
	a->upd_add(val);
	if(a->fa != null) 
	{
		a->fa->push_up();
		if(a->fa->fa != null)
			a->fa->fa->push_up();
	}//remember!!! 
}

void rev(int l, int r)
{
	get(l, r)->upd_rev();
} 

void del(int p)
{
	Node *a = get(p, p), *b = a->fa;
	a->fa = b->c[a->d()] = null, b->push_up();
	if(b->fa != null) b->fa->push_up();//remember!!! 
	size--;
}

int qry(int l, int r)
{
	return get(l, r)->minv;
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	#endif

	read(n);
	cur = pool;
	null = cur++, null->init();
	rep(i, 1, n)
	{
		int x;
		read(x);
		if(i == 1)
		{
			r = cur++;
			r->init();
			r->val = r->minv = x;
			r->sz = 1;size++;
		}
		else insert(r, i-1, x);
	}
	read(m);
	while(m--)
	{
		char q[10] = "\0";
		int a, b, c;
		scanf("%s", q);
		if(q[0] == 'A')
		{
			read(a), read(b), read(c);
			add(a, b, c);
		}
		else if(q[0] == 'R')
		{
			if(q[3] == 'E')
			{
				read(a), read(b);
				rev(a, b);
			}
			else
			{
				read(a), read(b), read(c);
				c %= b-a+1;if(c < 0) c += b-a+1;
				rev(a, b-c), rev(a, b), rev(a, a+c-1);
			}
		}
		else if(q[0] == 'I')
		{
			read(a), read(b);
			insert(r, a, b);
		}
		else if(q[0] == 'D')
		{
			read(a);
			del(a);
		}
		else
		{
			read(a), read(b);
			prt(qry(a, b)), puts("");
		}
	}

	#ifndef ONLINE_JUDGE
	fclose(stdin);
	fclose(stdout);
	#endif
	return 0;
}
