#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5+9;
const int MAXD = 2;
const int INF = 2e9;
const int MAXK = 29;

struct Node *null;
struct Node
{
	Node *c[2];
	int id, Max[MAXD], Min[MAXD];
	inline void init()
	{
		id = 0;
		c[0] = c[1] = null;
		for(int i = 0; i < MAXD; ++i)	
			Max[i] = -INF, Min[i] = INF;
	}
	inline void push_up()
	{
		for(int i = 0; i < 2; ++i)
			for(int j = 0; j < MAXD; ++j)
			{
				Max[j] = max(Max[j], c[i]->Max[j]);
				Min[j] = min(Min[j], c[i]->Min[j]);	
			}
	}
}pool[MAXN], *cur, *root;

struct Pot
{
	int id;
	long long p[MAXD];
	Pot()
	{
		memset(p, 0, sizeof p);
	}
	inline void read()
	{
		for(int i = 0; i < MAXD; ++i)
			scanf("%lld", p+i);	
	}
}node[MAXN];

int n, m;

int cmpd;

Pot q;
int k;

pair<long long,int> ans[MAXK];

template<class T> inline T sqr(T x)
{
	return x*x;	
}

inline bool cmp(const Pot &a, const Pot &b)
{
	return a.p[cmpd] < b.p[cmpd];
}

Node* make_tree(int l, int r, int d)
{
	Node *now = ++cur;now->init();
	if(l == r)
	{
		now->id = l;
		for(int i = 0; i < MAXD; ++i)
			now->Max[i] = now->Min[i] = node[l].p[i];
		return now;
	}
	int mid = (l+r)>>1;cmpd = d;
	if(l < mid) nth_element(node+l, node+mid-1, node+r+1, cmp);
	if(mid < r) nth_element(node+mid, node+mid, node+r+1, cmp);
	now->id = mid;
	for(int i = 0; i < MAXD; ++i)
		now->Max[i] = now->Min[i] = node[mid].p[i];
	if(l < mid)
		now->c[0] = make_tree(l, mid-1, d^1);
	if(mid < r)
		now->c[1] = make_tree(mid+1, r, d^1);
	now->push_up();
	return now;
}

inline void update(int id)
{
	long long dis = 0;
	for(int i = 0; i < MAXD; ++i)
		dis += sqr(node[id].p[i]-q.p[i]);
	pair<long long,int> g = make_pair(dis, -node[id].id);
	int pos = k;
	while(pos && ans[pos] < g) pos--;
	for(int i = k; i > pos; --i)
		ans[i+1] = ans[i];
	ans[pos+1] = g;
}

long long max_dist(Node *now)
{
	long long ret = 0;
	for(int i = 0; i < MAXD; ++i)
		ret += sqr(max(abs(now->Max[i]-q.p[i]), abs(now->Min[i]-q.p[i])));
	return ret;
}

void query(Node *now, int d)
{
	if(now == null)
		return ;
	update(now->id);
	long long dl = now->c[0]!=null?max_dist(now->c[0]):-1;
	long long dr = now->c[1]!=null?max_dist(now->c[1]):-1;
	if(dl >= dr)
	{
		if(dl >= ans[k].first) query(now->c[0], d^1);
		if(dr >= ans[k].first) query(now->c[1], d^1);
	}
	else
	{
		if(dr >= ans[k].first) query(now->c[1], d^1);
		if(dl >= ans[k].first) query(now->c[0], d^1);
	}
}

int main()
{
	freopen("jzpfar.in", "r", stdin);
	freopen("jzpfar.out", "w", stdout);
	
	scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
		node[i].read(), node[i].id = i;
	null = cur = pool, null->init();
	root = make_tree(1, n, 0);
	scanf("%d", &m);
	while(m--)
	{
		q.read();
		scanf("%d", &k);
		for(int i = 1; i <= k; ++i)
			ans[i].first = 0, ans[i].second = -n-1;
		query(root, 0);
		printf("%d\n", -ans[k].second);
	}
	
	return 0;
}
