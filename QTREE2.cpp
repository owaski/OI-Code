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

const int MAXN = 10009;

struct Node *null;
struct Node
{
	Node *fa, *c[2];
	int sum, dis, size, id;
	
	inline void init()
	{fa = c[0] = c[1] = null, sum = dis = size = id = 0;}
	
	inline void push_up()
	{sum = dis+c[0]->sum+c[1]->sum, size = c[0]->size+c[1]->size+1;}
	
	inline void setc(Node *p, int d)
	{c[d] = p, p->fa = this;}
	
	inline bool d()
	{return fa->c[1] == this;}
	
	inline bool is_root()
	{return fa == null || (fa->c[0] != this && fa->c[1] != this);}
	
	inline void rot()
	{
		Node *f = fa, *ff = fa->fa;
		int p = d(), pp = fa->d();
		if(f->is_root()) this->fa = ff;
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
};
Node pool[MAXN], *cur;
Node *node[MAXN];

int n;
struct Ugraph
{
	int size;
	int head[MAXN];
	int to[MAXN<<1];
	int w[MAXN<<1];
	int ne[MAXN<<1];
	
	inline void init()
	{size = 1, clr(head, 0);}
	
	inline void add_edge(int u, int v, int ww)
	{
		to[size] = v, w[size] = ww, ne[size] = head[u], head[u] = size++;
		to[size] = u, w[size] = ww, ne[size] = head[v], head[v] = size++;
	}
}G;
bool vis[MAXN];

void init()
{
	cur = pool, null = cur++, null->init();
	rep(i, 1, n)
	{
		node[i] = cur++;
		node[i]->init(), node[i]->size = 1, node[i]->id = i;
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
				node[to]->sum = node[to]->dis = G.w[i], node[to]->fa = node[now];
			}
	}
}

int query_dis(Node *a, Node *b)
{
	a->access();
	for(a = null; ; a = b, b = b->fa)
	{
		b->splay();
		if(b->fa == null) return a->sum+b->c[1]->sum;
		b->setc(a, 1), b->push_up();
	}
}

Node* kth(Node *p, int k)
{
	while(p->c[0]->size+1 != k)
	{
		if(p->c[0]->size >= k) p = p->c[0];
		else k -= p->c[0]->size+1, p = p->c[1];
	}
	return p;
}

int query_kth(Node *a, Node *b, int k)
{
	a->access();
	for(a = null; ; a = b, b = b->fa)
	{
		b->splay();
		if(b->fa == null)
		{
			if(b->c[1]->size+1 == k) return b->id;
			else if(b->c[1]->size >= k) return kth(b->c[1], b->c[1]->size+1-k)->id;
			else return kth(a, k-b->c[1]->size-1)->id; 
		}
		b->setc(a, 1), b->push_up();
	}
}

int main()
{
	open_init();

	int Test = read();
	while(Test--)
	{
		n = read();
		G.init(), init(), clr(vis, 0);
		rep(i, 1, n-1)
		{
			int u = read(), v = read(), w = read();
			G.add_edge(u, v, w);
		}
		bfs();
		char str[10] = "\0";
		while(scanf("%s", str))
		{
			if(str[1] == 'O') break;
			int a = read(), b = read(), c;
			if(str[0] == 'D')
				print(query_dis(node[a], node[b])), puts("");
			else
			{
				c = read();
				print(query_kth(node[a], node[b], c)), puts("");
			}
		}
	}

	close_file();
	return 0;
}
