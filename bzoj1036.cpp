#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
using namespace std;

template<class T> inline void read(T &x)
{
	int f = 1;char c;x = 0;
	while(((c=getchar()) < '0' || c > '9') && c != '-');
	if(c == '-') f = -1;else x = c-'0';
	while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
	x *= f;
}

template<class T> inline void prt(T x)
{
	if(!x) puts("0");
	else
	{
		if(x < 0)
		{
			x = -x;
			putchar('-');
		}
		int n = 0;
		char s[20] = "\0";
		while(x)
		{
			s[++n] = x%10+'0';
			x /= 10;	
		}
		per(i, n, 1)
			putchar(s[i]);
		puts("");
	}
}

const int MAXN = 30009;
const int INF = 1e9;

struct Lct *null;
struct Lct
{
	Lct *fa, *c[2];
	int val, sum, maxv;
	bool rev;
	inline void init(int p)
	{
		rev = false;
		val = sum = maxv = p;
		fa = c[0] = c[1] = null;
	}
	inline bool d()
	{
		return fa->c[1] == this;
	}
	inline bool is_root()
	{
		return fa == null || fa->c[0] != this && fa->c[1] != this;
	}
	inline void setc(Lct *p, int d)
	{
		c[d] = p, p->fa = this;
	}
	inline void push_up()
	{
		sum = c[0]->sum+c[1]->sum+val;
		maxv = max(val, max(c[0]->maxv, c[1]->maxv));
	}
	inline void upd_rev()
	{
		rev ^= true;
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
	}
	inline void rot()
	{
		Lct *f = fa, *ff = f->fa;
		int p = d(), pp = f->d();
		if(f->is_root()) fa = ff;
		else ff->setc(this, pp);
		f->setc(c[p^1], p), setc(f, p^1);
		f->push_up();	
	}
	inline void splay()
	{
		int n = 0;
		static Lct *anc[MAXN];
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
		push_up();
	}
	inline void access()
	{
		for(Lct *p = this, *q = null; p != null; q = p, p = p->fa)
			p->splay(), p->setc(q, 1), p->push_up();
	}
	inline void be_root()
	{
		access();
		splay();
		upd_rev();
	}
}pool[MAXN], *node[MAXN], *cur;

struct Ugraph
{
	int sz;
	int head[MAXN]; 
	int to[MAXN<<1];
	int ne[MAXN<<1];
	Ugraph()
	{
		sz = 1;
		memset(head, 0, sizeof head);
	}
	inline void add_edge(int u, int v)
	{
		to[sz] = v, ne[sz] = head[u], head[u] = sz++;
		to[sz] = u, ne[sz] = head[v], head[v] = sz++;
	}
}G;

int n;

inline void init()
{
	null = cur = pool;
	null->init(0), null->maxv = -INF;
}

inline void bfs()
{
	queue<int> q;
	static bool vis[MAXN];
	q.push(1), vis[1] = true;
	while(!q.empty())
	{
		int now = q.front();q.pop();
		for(int i = G.head[now]; i; i = G.ne[i])
			if(!vis[G.to[i]])
			{
				vis[G.to[i]] = true;
				node[G.to[i]]->fa = node[now];
				q.push(G.to[i]);
			}
	}
}

inline void modify(int u, int t)
{
	node[u]->splay();
	node[u]->val = t;
	node[u]->push_up();
}

inline int qmax(int u, int v)
{
	node[u]->be_root();
	node[v]->access(), node[v]->splay();
	return node[v]->maxv;
}

inline int qsum(int u, int v)
{
	node[u]->be_root();
	node[v]->access(), node[v]->splay();
	return node[v]->sum;
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1036.in", "r", stdin);
	freopen("bzoj1036.out", "w", stdout);
#endif

	read(n);
	init();
	rep(i, 1, n-1)
	{
		int u, v;
		read(u), read(v);
		G.add_edge(u, v);
	}
	rep(i, 1, n)
	{
		int w;
		read(w);
		node[i] = ++cur;
		node[i]->init(w);
	}
	bfs();
	int q;
	read(q);
	while(q--)
	{
		char s[10] = "\0";
		int a, b;
		scanf("%s", s);
		read(a), read(b);
		if(s[0] == 'C')
			modify(a, b);
		else if(s[1] == 'M')
			prt(qmax(a, b));
		else
			prt(qsum(a, b));
	}

	return 0;
}
