#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
using namespace std;

const int MAXN = 250009;

namespace sam
{
	const int SIZE = MAXN<<1;
	const int SIGMA = 26;
	struct Node
	{
		Node *fa, *ne[26];
		int l;
		inline void init(int len)
		{
			l = len, fa = 0;
			memset(ne, 0, sizeof ne);
		}
	}pool[SIZE], *cur, *root, *last;
	inline void init()
	{
		cur = root = last = pool;
		cur->init(0);
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
	inline int solve(char *s)
	{
		int ret = 0, len = 0;
		Node *now = root;
		while(*s)
		{
			int x = (*s++)-'a';
			while(now && !now->ne[x])
				now = now->fa;
			if(!now) now = root, len = 0;
			else
			{
				len = min(len, now->l)+1;
				now = now->ne[x];
				ret = max(ret, len);
			}
		}
		return ret;
	}
}

char s1[MAXN];
char s2[MAXN];

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("lcs.in", "r", stdin);
	freopen("lcs.out", "w", stdout);
	#endif
	
	sam::init();
	scanf("%s%s", s1, s2);
	int n = strlen(s1);
	rep(i, 0, n-1)
		sam::add(s1[i]-'a');
	printf("%d\n", sam::solve(s2));
	
	return 0;
}
