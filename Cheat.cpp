#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
using namespace std;

const int MAXN = 1100009;

int n, m, l;
char s[MAXN];
int q[MAXN], lq, rq;
int f[MAXN];

namespace sam
{
	const int SIZE = MAXN<<1;
	const int SIGMA = 3;
	struct Node
	{
		Node *fa, *ne[SIGMA];
		int l;
		inline void init(int len)
		{
			l = len, fa = 0;
			memset(ne, 0, sizeof ne);
		}
	}pool[SIZE], *root, *cur, *last;
	int pre[SIZE];
	inline void init()
	{
		cur = root = last = pool;
		cur->init(0), l = 0;
		memset(pre, 0, sizeof pre);
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
	inline void get()
	{
		int len = 0;
		Node *now = root;
		rep(i, 0, l-1)
		{
			int x = s[i]-'0';
			while(now && !now->ne[x])
				now = now->fa;
			if(!now) now = root, len = 0;
			else
			{
				len = min(len, now->l)+1;
				now = now->ne[x];
			}
			pre[i+1] = len;
		}
	}
	inline bool check(int L)
	{
		q[lq=rq=1] = 0;
		rep(i, 0, l) f[i] = 0;
		rep(i, L, l)
		{
			f[i] = f[i-1];
			while(lq <= rq && q[lq] < i-pre[i]) ++lq;
			if(pre[i] >= L)
			{
				if(pre[i] > i-q[lq]) f[i] = max(f[i], f[q[lq]]+i-q[lq]);
				else f[i] = max(f[i], f[i-pre[i]]+pre[i]);
			}
			while(lq <= rq && f[i-L+1]-(i-L+1) >= f[q[rq]]-q[rq]) --rq;
			q[++rq] = i-L+1;
		}
		return f[l]*10 >= l*9;
	}
	inline int solve()
	{
		rep(i, 0, l-1) 
			pre[i] = 0;
		sam::get();
		int le = 0, ri = l;
		while(le < ri)
		{
			int mid = (le+ri+1)>>1;
			if(check(mid)) le = mid;
			else ri = mid-1;
		}
		return le;
	}
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("Cheat.in", "r", stdin);
	freopen("Cheat.out", "w", stdout);
	#endif
	
	sam::init();
	scanf("%d%d", &n, &m);
	while(m--)
	{
		scanf("%s", s);
		l = strlen(s);
		rep(i, 0, l-1) sam::add(s[i]-'0');
		sam::add(2);
	}
	while(n--)
	{
		scanf("%s", s);
		l = strlen(s);
		printf("%d\n", sam::solve());
	}
	
	return 0;
}
