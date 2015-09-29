#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100009;
const int MAXC = 10;
const int MAXLEAF = 21; 

int n, c;
int sgn[MAXN];
int deg[MAXN];
int leaf;
int q[MAXN*MAXLEAF], h, t;

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

struct Trie
{
	int sz, root;
	int ne[MAXN*MAXLEAF][MAXC];
	Trie()
	{
		root = sz = 0;
		memset(ne[0], 0, sizeof ne[0]);
	}
	inline int add(int u, int v)
	{
		if(!ne[u][v])
			ne[u][v] = ++sz;
		return ne[u][v];
	}
}T;

namespace Sam
{
	const int SZ = MAXN*MAXLEAF<<1;
	struct Node *null;
	struct Node
	{
		int l;
		Node *fa, *ne[MAXC];
		inline void init(int len)
		{
			l = len, fa = 0;
			memset(ne, 0, sizeof ne);	
		}
	}pool[SZ], *root, *cur, *last;
	int cnt[SZ];
	Node *q[SZ];
	long long pos[SZ];
	int f[SZ];
	inline void init()
	{
		last = root = cur = pool;
		cur->init(0);
	}
	inline Node* newnode(int len)
	{
		Node *tmp = ++cur;
		tmp->init(len);
		return tmp;
	}
	inline void add(Node *p, int x)
	{
		Node *np = newnode(p->l+1);
		while(p && !p->ne[x])
			p->ne[x] = np, p = p->fa;
		if(!p) np->fa = root;
		else
		{
			Node *q = p->ne[x];
			if(q->l == p->l+1) np->fa = q;
			else
			{
				Node *r = newnode(p->l+1);
				memcpy(r->ne, q->ne, sizeof r->ne);
				r->fa = q->fa, q->fa = np->fa = r;
				while(p && p->ne[x] == q)
					p->ne[x] = r, p = p->fa;	
			}
		}
		last = np;
	}
	inline void Top()
	{
		int Max = 0;
		for(int i = 0; i <= cur-pool; ++i)
			Max = max(Max, pool[i].l), cnt[pool[i].l]++;
		for(int i = 1; i <= Max; ++i)
			cnt[i] += cnt[i-1];
		for(int i = 0; i <= cur-pool; ++i)
			q[--cnt[pool[i].l]] = &pool[i], pos[i] = cnt[pool[i].l];
	}
	inline void dp()
	{
		Top();
		f[0] = 1;
		for(int i = 0; i <= cur-pool; ++i)
			for(int j = 0; j < c; ++j)
				if(q[i]->ne[j])
					f[pos[q[i]->ne[j]-pool]] += f[i];
	}
	inline long long get()
	{
		long long ret = 0;
		for(int i = 1; i <= cur-pool; ++i)
			ret += f[i];
		return ret;
	}
}

void dfs(int now, int p, int fa)
{
	p = T.add(p, sgn[now]);
	for(int i = G.head[now]; i; i = G.ne[i])
		if(G.to[i] != fa)
			dfs(G.to[i], p, now);
}

inline void bfs()
{
	Sam::init();
	h = -1, q[t=0] = T.root;
	Sam::q[0] = Sam::root;
	while(h < t)
	{
		int now = q[++h];
		Sam::Node *p = Sam::q[h];
		for(int i = 0; i < c; ++i)
			if(T.ne[now][i])
			{
				Sam::add(p, i);
				q[++t] = T.ne[now][i];
				Sam::q[t] = Sam::last;
			}
	}
}

int main()
{
	freopen("bzoj3926.in", "r", stdin);
	freopen("bzoj3926.out", "w", stdout);
	
	scanf("%d%d", &n, &c);
	for(int i = 1; i <= n; ++i)
		scanf("%d", sgn+i);
	for(int i = 1; i < n; ++i)
	{
		int u, v;
		scanf("%d%d", &u, &v);
		G.add_edge(u, v);	
		deg[u]++, deg[v]++;
	}
	for(int i = 1; i <= n; ++i)
		if(deg[i] == 1)
			dfs(i, T.root, 0);
	bfs();
	Sam::dp();
	cout << Sam::get() << endl;
	
	return 0;	
}
