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
#define pb push_back
#define mp make_pair
#define clr(a, b) memset(a, b, sizeof a)
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
typedef long long LL;
typedef double DB;
typedef long double LD;
using namespace std;

void open_init()
{
	#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	#endif
}

void close_file()
{
	#ifndef ONLINE_JUDGE
	fclose(stdin);
	fclose(stdout);
	#endif
}

const int MAXN = 10009;

struct Node *null;
struct Node
{
	Node *fa, *c[2];
	int Max, key;
	
	inline void init()
	{fa = c[0] = c[1] = null, Max = key = 0;}
	
	inline void push_up()
	{Max = max(key, max(c[0]->Max, c[1]->Max));}
	
	inline void setc(Node *p, int d)
	{c[d] = p, p->fa = this;}
	
	inline bool d()
	{return fa->c[1] == this;}
	
	inline bool is_root()
	{return fa == null || (fa->c[0] != this && fa->c[1] != this);}
	
	inline void rot()
	{
		Node *f = fa, *ff = fa->fa;
		int p = d(), pf = fa->d();
		if(f->is_root()) fa = ff;
		else ff->setc(this, pf);
		f->setc(c[p^1], p), setc(f, p^1);
		f->push_up();
	}
	
	inline Node* splay()
	{
		while(!is_root())
		{
			if(!fa->is_root())
				if(fa->d() == d()) fa->rot();
				else rot();
			rot();
		}
		push_up();
		return this;
	}
	
	inline Node* access()
	{
		for(Node *p = this, *q = null; p != null; q = p, p = p->fa)
		{
			p->splay()->setc(q, 1);
			p->push_up();
		}
		return splay();
	}
};
Node pool[MAXN], *cur;
Node *node[MAXN];
int n;
int E[MAXN];
bool vis[MAXN];
struct Ugraph
{
	int size;
	int head[MAXN];
	int to[MAXN<<1];
	int w[MAXN<<1];
	int ne[MAXN<<1];
	
	void init() 
	{size = 1;clr(head, 0);}
	
	void add_edge(int u, int v, int ww)
	{
		to[size] = v, w[size] = ww, ne[size] = head[u], head[u] = size++;
		to[size] = u, w[size] = ww, ne[size] = head[v], head[v] = size++;	
	}
}G;

void init()
{
	cur = pool;
	null = cur++;
	null->init();
	rep(i, 1, n)
	{
		node[i] = cur++;
		node[i]->init();
	}
}

void bfs()
{
	queue<int> q;
	clr(vis, false), q.push(1), vis[1] = true;
	while(!q.empty())
	{
		int now = q.front(), to;q.pop();
		for(int i = G.head[now]; i; i = G.ne[i])
			if(!vis[to = G.to[i]])
			{
				vis[to] = true, q.push(to), E[(i+1)>>1] = to;
				node[to]->key = G.w[i], node[to]->push_up(), node[to]->fa = node[now];
			}
	}
}

int query(Node *a, Node *b)
{
	a->access(), a = null;
	while(true)
	{
		b->splay();
		if(b->fa == null)
			return max(b->c[1]->Max, a->Max);
		b->setc(a, 1), b->push_up();
		a = b, b = b->fa;
	}
}

int main()
{
	open_init();

	int Test;
	scanf("%d", &Test);
	while(Test--)
	{
		scanf("%d", &n);
		init();G.init();
		rep(i, 1, n-1)
		{
			int u, v, w;
			scanf("%d%d%d", &u, &v, &w);
			G.add_edge(u, v, w);
		}
		bfs();
		char str[10] = "\0";
		while(scanf("%s", str))
		{
			if(str[0] == 'D') break;
			int a, b;scanf("%d%d", &a, &b);
			if(str[0] == 'Q')
				printf("%d\n", query(node[a], node[b]));
			else 
			{
				node[E[a]]->splay()->key = b;
				node[E[a]]->push_up();
			}
		}
	}

	close_file();
	return 0;
}
