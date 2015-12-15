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
#define per(i, a, b) for(int i = (a); i >= (b); --i)
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

int read()
{
	char c;
	int x = 0;
	while((c=getchar()) < '0' || c > '9');
	x = c-'0';
	while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
	return x;
}

void print(int x)
{
	if(!x) putchar('0');
	else 
	{
		int top = 0;
		char str[20] = "\0";
		while(x)
		{
			str[++top] = x%10+'0';
			x /= 10;
		}
		per(i, top, 1)
			putchar(str[i]);
	}
}

const int MAXN = 100009;

struct Node *null;
struct Node
{
	Node *fa, *c[2];
	int sum, id;
	bool col;
	
	inline void init()
	{fa = c[0] = c[1] = null, sum = id = col = 0;}
	
	inline void push_up()
	{sum = col+c[0]->sum+c[1]->sum;}
	
	inline void setc(Node *p, int d)
	{c[d] = p, p->fa = this;}
	
	inline bool d()
	{return fa->c[1] == this;}
	
	inline bool is_root()
	{return fa == null || (fa->c[0] != this && fa->c[1] != this);}
	
	inline void rot()
	{
		Node *f = fa, *ff = fa->fa;
		int p = d(), pp = f->d();
		if(f->is_root()) fa = ff;
		else ff->setc(this, pp);
		f->setc(c[p^1], p), setc(f, p^1);
		f->push_up();
	}
	
	inline Node* splay()
	{
		while(!is_root())
		{
			if(!fa->is_root())
				if(d() == fa->d()) fa->rot();
				else rot();
			rot();
		}
		push_up();
		return this;
	}
	
	inline Node* access()
	{
		for(Node *p = this, *q = null; p != null; q = p, p = p->fa)
			p->splay()->setc(q, 1), p->push_up(); 
		return splay();
	}
}pool[MAXN], *cur;
Node *node[MAXN];

int n, q;
struct Ugraph
{
	int size;
	int head[MAXN];
	int to[MAXN<<1];
	int ne[MAXN<<1];
	
	inline void init()
	{size = 1, clr(head, 0);}
	
	inline void add_edge(int u, int v)
	{
		to[size] = v, ne[size] = head[u], head[u] = size++;
		to[size] = u, ne[size] = head[v], head[v] = size++;
	}
}G;
bool vis[MAXN];

void init()
{
	cur = pool, null = cur++, null->init();
	rep(i, 1, n)
	{
		node[i] = cur++, node[i]->init();
		node[i]->id = i;
	}
}

void bfs()
{
	queue<int> q;
	q.push(1), vis[1] = true;
	while(!q.empty())
	{
		int now = q.front(), to;q.pop();
		for(int i = G.head[now]; i; i = G.ne[i])
			if(!vis[to = G.to[i]])
			{
				vis[to] = true, q.push(to);
				node[to]->fa = node[now];
			}
	}
}

int query(Node *a)
{
	a->access();
	while(a->sum)
	{
		if(a->c[1]->sum) a = a->c[1];
		else if(a->col) return a->id;
		else a = a->c[0];
	}
	return -1;
}

int main()
{
	open_init();
	
	n = read(), q = read();
	G.init(), init();
	rep(i, 1, n-1)
	{
		int u = read(), v = read();
		G.add_edge(u, v);
	}
	bfs();
	while(q--)
	{
		int a = read(), b = read();
		if(!a)
		{
			node[b]->splay()->col ^= 1;
			node[b]->push_up();
		}
		else printf("%d\n", query(node[b]));
	}
	
	close_file();
	return 0;
}
