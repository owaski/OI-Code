#include <cstdio>
#include <cstring>
#include <algorithm>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
#define clr(a, b) memset(a, b, sizeof a)
using namespace std;

template<class T> inline void read(T &x)
{
	char c;int f = 1;x = 0;
	while(((c=getchar()) < '0' || c > '9') && c != '-');
	if(c == '-') f = -1;else x = c-'0';
	while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
	x *= f;
}

const int MAXN = 60009;
const int MAXM = 10009;
const int LOGN = 16;

struct Node *null;
struct Node
{
	Node *fa, *c[2];
	unsigned short sum, val, cnt;
	inline void init(int v)
	{
		val = v, sum = cnt = 0;
		fa = c[0] = c[1] = null;
	}
	inline void setc(Node *p, int d)
	{
		c[d] = p, p->fa = this;
	}
	inline bool d()
	{
		return fa->c[1] == this;
	}
	inline void push_up()
	{
		sum = c[0]->sum+c[1]->sum+cnt;
	}
	inline void rot()
	{
		Node *f = fa, *ff = f->fa;
		int p = d(), pp = f->d();
		if(ff == null) fa = ff;
		else ff->setc(this, pp);
		f->setc(c[p^1], p), setc(f, p^1);
		f->push_up();
	}
	inline void splay()
	{
		while(fa != null)
		{
			Node *f = fa;
			if(f->fa != null)
				if(d() == f->d()) f->rot();
				else rot();
			rot();
		}
		push_up();
	}
};

int n, m;
int a[MAXN];
int q[MAXM][4];
int val[MAXN], tot;

Node pool[MAXN*LOGN], *cur;
Node *root[MAXN<<2];

Node* newnode(int val)
{
	Node *tmp = cur++;
	tmp->init(val);
	return tmp;
}

void build(int x, int l, int r)
{
	root[x] = null;
	if(l == r) return ;
	int mid = (l+r)>>1;
	build(x<<1, l, mid);
	build(x<<1|1, mid+1, r);
}

inline int id(int x)
{
	return lower_bound(val+1, val+tot+1, x)-val;
}

void ins2(Node *&r, int val, int add)
{
	if(r == null)
	{
		r = newnode(val);
		r->cnt += add;
		r->sum += add;
		return ;
	}
	r->splay();
	Node *now = r;
	while(true)
	{
		now->sum += add;
		if(now->val == val)
		{
			now->cnt += add;
			now->splay();
			break;
		}
		int d = val > now->val;
		if(now->c[d] == null)
		{
			Node *&tmp = now->c[d];
			tmp = newnode(val);
			tmp->fa = now;
			tmp->cnt += add;
			tmp->sum += add;
			tmp->splay();
			break;
		}
		else now = now->c[d];
	}
}

void ins(int x, int l, int r, int val, int p, int add)
{
	ins2(root[x], p, add);
	if(l == r) return ;
	int mid = (l+r)>>1;
	if(val <= mid) ins(x<<1, l, mid, val, p, add);
	else ins(x<<1|1, mid+1, r, val, p, add);
}

int calc(Node *now, int p)
{
	if(now == null)
		return 0;
	if(now->val <= p)
		return now->c[0]->sum+now->cnt+calc(now->c[1], p);
	else return calc(now->c[0], p);
}

int qry2(Node *root, int l, int r)
{
	if(root == null) return 0;
	root->splay();
	return calc(root, r)-calc(root, l-1);
}

int qry(int ql, int qr, int k)
{
	int l = 1, r = tot, x = 1;
	while(l < r)
	{
		int mid = (l+r)>>1;
		int tmp = qry2(root[x<<1], ql, qr);
		if(tmp >= k) r = mid, x <<= 1;
		else k -= tmp, l = mid+1, x = x<<1|1;
	}
	return l;
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("kth.in", "r", stdin);
	freopen("kth.out", "w", stdout);
	#endif
	
	int Case;
	read(Case);
	cur = pool;
	null = newnode(0);
	while(Case--)
	{
		tot = 0;
		cur = pool+1;
		read(n), read(m);
		rep(i, 1, n)
		{
			read(a[i]);
			val[++tot] = a[i];
		}
		rep(i, 1, m)
		{
			char c[3] = "\0";
			scanf("%s", c);
			read(q[i][1]), read(q[i][2]);
			if(c[0] == 'Q') 
			{
				read(q[i][3]);
				q[i][0] = 0;
			}
			else 
			{
				q[i][0] = 1;
				val[++tot] = q[i][2];
			}
		}
		sort(val+1, val+tot+1);
		build(1, 1, tot);
		rep(i, 1, n)
			ins(1, 1, tot, id(a[i]), i, 1);
		rep(i, 1, m)
			if(q[i][0] == 0)
				printf("%d\n", val[qry(q[i][1], q[i][2], q[i][3])]);
			else
			{
				ins(1, 1, tot, id(a[q[i][1]]), q[i][1], -1);
				ins(1, 1, tot, id(q[i][2]), q[i][1], 1);
				a[q[i][1]] = q[i][2];
			}
	}

	return 0;
}
