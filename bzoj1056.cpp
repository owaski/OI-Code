#include <bits/stdc++.h>
using namespace std;

const int N = 250009;
const int L = 12;

map<long long,int> g;
int tot_name;
char name[N][L];
int T[N];
long long val[N];

namespace Hash
{
	inline long long get(char *s)
	{
		long long ret = 0;
		while(isalpha(*s))
			ret = ret*27+(*s++)-'A'+1;
		return ret;
	}
}

namespace Splay
{
	struct Data
	{
		long long v;int t;
		Data() {}
		Data(long long _v, int _t):v(_v), t(_t) {}
		inline friend bool operator < (const Data &a, const Data &b)
		{
			return a.v > b.v || a.v == b.v && a.t < b.t;
		}
		inline friend bool operator == (const Data &a, const Data &b)
		{
			return a.v == b.v && a.t == b.t;	
		}
		inline friend bool operator <= (const Data &a, const Data &b)
		{
			return a < b || a == b;	
		}
	};
	struct Node
	{
		int id, sz;
		Data key;
		Node *c[2], *fa;
		Node() {}
		Node(const Data &k, int _id)
		{
			key = k, id = _id, sz = 1;
			c[0] = c[1] = fa = 0;	
		}
		inline void push_up()
		{
			sz = 1;
			if(c[0]) sz += c[0]->sz;
			if(c[1]) sz += c[1]->sz;	
		}
		inline void setc(Node *p, int d)
		{
			c[d] = p;
			if(p) p->fa = this;	
		}
		inline bool d()
		{
			if(fa) return fa->c[1] == this;	
			else return 0;
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
			while(this != r)
			{
				if(fa != r)
				{
					if(d() == fa->d()) fa->rot(r);
					else rot(r);
				}
				rot(r);
			}
			push_up();
		}
	}pool[N], *cur = pool, *root = 0;
	
	int st[12], top;
	
	inline Node* lower_bound(const Data &key)
	{
		Node *now = root;
		while(true)
		{
			int d = now->key <= key;
			if(now->key == key || !now->c[d]) return now;
			else now = now->c[d];	
		}
		return 0;
	}
	
	inline Node* rank(int rk)
	{
		Node *now = root;
		while(true)
		{
			int s = now->c[0]?now->c[0]->sz:0;
			if(rk <= s) now = now->c[0];
			else if(rk == s+1) return now;
			else rk -= s+1, now = now->c[1];
		}
	}
	
	inline void del(const Data &key)
	{
		Node *p = lower_bound(key);
		p->splay(root);
		if(p->sz == 1) root = 0;
		else if(p->c[0])
		{
			Node *q = rank(p->c[0]->sz);
			q->splay(p->c[0]), q->setc(p->c[1], 1);
			q->push_up(), q->fa = 0, root = q;
		}
		else
		{
			root = p->c[1];
			root->fa = 0;	
		}
	}
	
	bool flag;
	void ins(Node *&now, const Data &key, int id)
	{
		if(!now)
		{
			now = ++cur, flag = true;
			*now = Node(key, id);
			return ;
		}
		int d = now->key < key;
		ins(now->c[d], key, id);
		if(flag) now->c[d]->fa = now, flag = false;
		now->push_up();
	}
	
	void get_rank(Node *&p)
	{
		if(top == 10) return ;
		if(!p) return ;
		get_rank(p->c[0]);
		if(top < 10) st[++top] = p->id;
		get_rank(p->c[1]);	
	}
	
	inline void query_rank(int rk)
	{
		top = 0;
		Node *p = rank(rk);
		p->splay(root);
		st[++top] = p->id;
		get_rank(p->c[1]);
	}
	
	inline int query_name(const Data &key)
	{
		Node *p = lower_bound(key);
		p->splay(root);int ret = p->c[0]?p->c[0]->sz+1:1;
		return ret;	
	}
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1056.in", "r", stdin);
	freopen("bzoj1056.out", "w", stdout);
#endif

	int n = 0, id, len;
	long long hv, v;
	scanf("%d", &n);
	using namespace Splay;
	using namespace Hash;
	for(int t = 1; t <= n; ++t)
	{
		char s[L] = "\0";
		scanf("%s", s);
		if(s[0] == '+')
		{
			hv = get(s+1);
			scanf("%lld", &v);
			if(g.count(hv)) 
			{
				id = g[hv];
				del(Data(val[id], T[id]));
			}
			else 
			{
				id = g[hv] = ++tot_name;
				strcpy(name[id], s+1);
			}
			val[id] = v, T[id] = t, flag = false;
			ins(root, Data(val[id], T[id]), id);
			cur->splay(root);
		}
		else if(isdigit(s[1]))
		{
			id = 0;
			len = strlen(s+1);
			for(int i = 1; i <= len; ++i)
				id = id*10+s[i]-'0';
			top = 0;
			query_rank(id);
			printf("%s", name[st[1]]);
			for(int i = 2; i <= top; ++i)
				printf(" %s", name[st[i]]);
			puts("");
		}
		else
		{
			hv = get(s+1), id = g[hv];
			printf("%d\n", query_name(Data(val[id], T[id])));
		}
	}

	return 0;	
}
