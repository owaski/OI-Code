#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
using namespace std;

const int MAXN = 100009;
const int INF = 1e9;
const int T = 10;

namespace sam
{
	const int SIZE = MAXN<<1;
	const int SIGMA = 26;
	struct Node
	{
		Node *fa, *ne[SIGMA];
		int l, tmp, ans;
		inline void init(int len)
		{
			l = len, tmp = 0, ans = INF;
			fa = 0, memset(ne, 0, sizeof ne);
		}
	}pool[SIZE], *cur, *last, *root;
	inline void init()
	{
		root = cur = last = pool;
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
	inline void modify(char *s)
	{
		int len = 0;
		Node *now = root;
		while(*s)
		{
			int x = (*s++)-'a';
			while(now && !now->ne[x])
			{
				now = now->fa;
				if(now)
					now->tmp = max(now->tmp, now->l);//没加这句话 WA了一次 
			}
			if(!now) now = root, len = 0;
			else
			{
				len = min(len, now->l)+1;
				now = now->ne[x];
				now->tmp = max(now->tmp, len);
			}
		}
		rep(i, 1, cur-pool)
		{
			pool[i].ans = min(pool[i].ans, pool[i].tmp);
			pool[i].tmp = 0;
		}
	}
	inline int solve()
	{
		int ans = 0;
		rep(i, 1, cur-pool)
			ans = max(ans, pool[i].ans);
		return ans;
	}
}

char s[MAXN];

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("lcs2.in", "r", stdin);
	freopen("lcs2.out", "w", stdout);
	#endif
	
	sam::init();
	scanf("%s", s);
	int n = strlen(s);
	rep(i, 0, n-1)
		sam::add(s[i]-'a');
	while(scanf("%s", s) != EOF)
		sam::modify(s);
	printf("%d\n", sam::solve());

	return 0;
}
