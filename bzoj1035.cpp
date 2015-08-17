#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
#define clr(a, b) memset(a, b, sizeof a)
#define debug(...) fprintf(stderr, __VA_ARGS__)
#define pb push_back
#define mp make_pair
#define pii pair<int,int>
#define X first
#define Y second
typedef double DB;
using namespace std;

const int MAXN = 609;
const int MAXM = 8009;
const DB pi = acos(-1.0);
const DB eps = 1e-5;

struct Pot
{
	DB x, y;
	Pot() {x = y = 0;}
	Pot(DB _x, DB _y):x(_x),y(_y) {}
	
	inline void read()
	{
		scanf("%lf%lf", &x, &y);
	}
	
	inline friend Pot operator + (const Pot &a, const Pot &b)
	{
		return Pot(a.x+b.x, a.y+b.y);
	}
	inline friend Pot operator - (const Pot &a, const Pot &b)
	{
		return Pot(a.x-b.x, a.y-b.y);
	}
	inline friend DB operator * (const Pot &a, const Pot &b)
	{
		return a.x*b.x+a.y*b.y;
	}
	inline friend DB operator ^ (const Pot &a, const Pot &b)
	{
		return a.x*b.y-a.y*b.x;
	}
	inline friend Pot operator * (const Pot &a, DB k)
	{
		return Pot(a.x*k, a.y*k);
	}
	inline friend Pot operator / (const Pot &a, DB k)
	{
		return Pot(a.x/k, a.y/k);
	}
	inline friend bool operator < (const Pot &a, const Pot &b)
	{
		return a.x < b.x || (a.x == b.x && a.y < b.y);
	}
};

int n, m;
Pot army[MAXN];
Pot pot[MAXM];
map<Pot,int> node;
int cnt;
int next[MAXM];
pii e[MAXM];
vector<int> g[MAXM];
int vis[MAXM];
int id[MAXM], tot;
int ch[MAXM];
pair<DB,int> area[MAXN];
vector<int> ring[MAXN];
int iarmy[MAXN];
vector<int> ans[MAXN];
bool flag[MAXN];

inline int inv(int x)
{
	return ((x-1)^1)+1;
}

inline DB get_k(int a, int b)
{
	Pot p = pot[a];
	Pot q = pot[b];
	p = q-p;
	return atan2(p.y, p.x);
}

inline DB len(const Pot &a)
{
	return sqrt(a.x*a.x+a.y*a.y);
}

inline DB get_angle(const Pot &a, const Pot &b)
{
	return acos(a*b/len(a)/len(b));
}

inline bool cmp(int a, int b)
{
	DB a1 = get_k(e[a].X, e[a].Y);
	DB a2 = get_k(e[b].X, e[b].Y);
	return a1 < a2;
}

DB dfs(int now)
{
	id[now] = tot;
	vis[now] = true;
	ring[tot].pb(now);
	int u = e[now].X, v = e[now].Y, ne;
	vector<int> p = g[v];
	sort(p.begin(), p.end(), cmp);
	int SZ = p.size();
	rep(i, 0, SZ-1)
		if(e[p[i]].Y == u)
		{
			ne = p[(i-1+SZ)%SZ];
			break;
		}
	next[now] = ne;
	DB ret = pot[u]^pot[v];
	if(!vis[ne]) ret += dfs(ne);
	return ret;
}

inline bool intersect(const Pot &a, const Pot &b, const Pot &c, const Pot &d)
{
	return ((b-a)^(c-a))*((b-a)^(d-a)) < 0 && ((d-c)^(a-c))*((d-c)^(b-c)) < 0;
}

bool inside(int a, int b)
{
	Pot x = army[a], y = Pot(1e9+7, 998244353);
	vector<int> r = ring[b];
	int SZ = r.size(), cnt = 0;
	rep(i, 0, SZ-1)
		if(intersect(x, y, pot[e[r[i]].Y], pot[e[r[(i+1)%SZ]].Y]))
			cnt++;
	return cnt & 1;
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen("Risk.in", "r", stdin);
	freopen("Risk.out", "w", stdout);
	#endif
	
	scanf("%d%d", &n, &m);
	rep(i, 1, n)
		army[i].read();
	rep(i, 1, m)
	{
		Pot x, y;
		x.read(), y.read();
		int p, q;
		if(node.count(x)) p = node[x];
		else p = node[x] = ++cnt, pot[cnt] = x;
		if(node.count(y)) q = node[y];
		else q = node[y] = ++cnt, pot[cnt] = y;
		int a = (i<<1)-1;
		int b = i<<1;
		g[p].pb(a), g[q].pb(b);
		e[a] = mp(p, q), e[b] = mp(q, p);
	}
	rep(i, 1, m<<1)
		if(!vis[i])
		{
			area[++tot] = mp(dfs(i), tot);
			if(area[tot].X < 0)
			{
				int SZ = ring[tot].size();
				rep(j, 0, SZ-1)
					id[ring[tot][j]] = 0;
				ring[tot].clear();
				area[tot--] = mp(0, 0);
			}
		}
	sort(area+1, area+tot+1);
	rep(i, 1, tot)
		ch[area[i].Y] = i;
	rep(i, 1, n)
		rep(j, 1, tot)
			if(inside(i, area[j].Y))
			{
				iarmy[j] = i;
				break;
			}
	rep(i, 1, m<<1)
		if(id[i] && id[inv(i)])
			ans[iarmy[ch[id[i]]]].pb(iarmy[ch[id[inv(i)]]]);
		else if(id[i]) flag[ch[id[i]]] = true;
	rep(i, 1, tot)
		if(flag[i])
			rep(j, i+1, tot)
				if(i != j)
					if(inside(iarmy[i], area[j].Y))
					{
						ans[iarmy[i]].pb(iarmy[j]);
						ans[iarmy[j]].pb(iarmy[i]);
						break;
					}
	rep(i, 1, n)
	{
		sort(ans[i].begin(), ans[i].end());
		vector<int>::iterator end = unique(ans[i].begin(), ans[i].end());
		int sum = distance(ans[i].begin(), end);
		printf("%d", sum);
		rep(j, 0, sum-1) printf(" %d", ans[i][j]);
		puts("");
	}

	return 0;
}
