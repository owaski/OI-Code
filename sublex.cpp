#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
using namespace std;

const int MAXN = 90009;
const int SZ = MAXN<<1;
const int SIGMA = 26;

namespace sam
{
	struct Node
	{
		Node *fa, *ne[SIGMA];
		int l, sum;
		inline void init(int len)
		{
			l = len, sum = 0;
			memset(ne, 0, sizeof ne), fa = 0;
		}
	}pool[SZ], *cur, *last, *root;
	int c[SZ];Node *a[SZ];
	char s[MAXN];
	inline void init()
	{
		root = last = cur = pool;
		root->init(0);
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
	inline void pre()
	{
		int Max = 0;
		rep(i, 0, cur-pool)
			c[pool[i].l]++, Max = max(Max, pool[i].l);
		rep(i, 1, Max) c[i] += c[i-1];
		rep(i, 0, cur-pool)
			a[--c[pool[i].l]] = pool+i;
		per(i, cur-pool, 0)
		{
			Node *p = a[i];
			rep(j, 0, SIGMA-1)
				if(p->ne[j])
					p->sum += p->ne[j]->sum+1;
		}
	}
	inline void solve(int p)
	{
		Node *now = root;
		while(true)
		{
			rep(i, 0, SIGMA-1)
				if(now->ne[i])
				 	if(p == 1)
					{
						putchar('a'+i);
						return ;
					}
					else if(p-1 <= now->ne[i]->sum)
					{
						p--;
						now = now->ne[i];
						putchar('a'+i);
						break;
					}
					else p -= now->ne[i]->sum+1;
		}
	}
}

char s[MAXN];

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("sublex.in", "r", stdin);
	freopen("sublex.out", "w", stdout);
	#endif
	
	int Case;
	sam::init();
	scanf("%s", s);
	int n = strlen(s);
	rep(i, 0, n-1)
		sam::add(s[i]-'a');
	sam::pre();
	scanf("%d", &Case);
	while(Case--)
	{
		int k;
		scanf("%d", &k);
		sam::solve(k), puts("");
	}
	
	return 0;
}
