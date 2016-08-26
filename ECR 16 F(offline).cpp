#include <bits/stdc++.h>

#define ll long long
#define uint unsigned

#define debug(...) fprintf(stderr, __VA_ARGS__)

#define SZ(x) ((int)(x).size())
#define pb push_back

#define pii std::pair<int,int>
#define mp std::make_pair
#define fi first
#define se second

template<class T>inline void chkmax(T &x, const T &y) {if(x < y) x = y;}
template<class T>inline void chkmin(T &x, const T &y) {if(y < x) x = y;}

template<class T>
inline void read(T &x) {
	char c;int f = 1;x = 0;
	while(((c = getchar()) < '0' || c > '9') && c != '-');
	if(c == '-') f = -1;else x = c - '0';
	while((c = getchar()) >= '0' && c <= '9') x = x * 10 + c - '0';
	x *= f;
}

const int M = 3e5, L = M;
const int SIGMA = 26;

char s[L + 9];
struct data {
	int t;
	std::string s;
	void input() {
		read(t);
		scanf("%s", ::s);
		for(int i = 0, l = strlen(::s); i < l; ++i)
			s += ::s[i];
	}
}q[M + 9];
int ans[M + 9];

std::map<int, int> id;
std::vector<int> str[M * 4 + 9];

struct acam {
	int sz, rt, tr[L + 9][SIGMA], val[L + 9], sum[L + 9], f[L + 9];
	int newnode() {
		int ret = ++sz;
		memset(tr[ret], 0, sizeof tr[ret]);
		val[ret] = sum[ret] = f[ret] = 0;
		return ret;
	}
	void init() {
		sz = 0, rt = newnode();
	}
	void add(const std::string &s) {
		int cur = rt;
		for(int i = 0; i < SZ(s); ++i) {
			int x = s[i] - 'a';
			if(!tr[cur][x]) tr[cur][x] = newnode();
			cur = tr[cur][x];
		}
		val[cur]++;
	}
	void build() {
		static int q[L + 9], l, r;
		l = r = 0, q[++r] = rt;
		while(l < r) {
			int now = q[++l];
			for(int i = 0; i < SIGMA; ++i)
				if(tr[now][i]) {
					int nex = tr[now][i], x = f[now];
					while(x && !tr[x][i]) x = f[x];
					if(x) f[nex] = tr[x][i];
					else f[nex] = rt;
					q[++r] = nex, sum[nex] = sum[f[nex]] + val[nex];
				}
				else {
					if(f[now]) tr[now][i] = tr[f[now]][i];
					else tr[now][i] = rt;
				}
		}
	}
	int query(const std::string &s) {
		int ret = 0, cur = rt;
		for(int i = 0; i < SZ(s); ++i) {
			int x = s[i] - 'a';
			cur = tr[cur][x];
			ret += sum[cur];
		}
		return ret;
	}
}ac;

int gethash(const std::string &s) {
	int val = 0;
	for(int i = 0; i < SZ(s); ++i)
		val = (val * 31ll + (s[i] - 'a' + 1)) % 998244353;
	return val;
}

void ins(int x, int l, int r, int tl, int tr, int v) {
	if(tl <= l && r <= tr) {
		str[x].pb(v);
		return ;
	}
	int mid = (l + r) >> 1;
	if(tl <= mid) ins(x << 1, l, mid, tl, tr, v);
	if(tr > mid) ins(x << 1 | 1, mid + 1,  r, tl, tr, v);
}

void dfs(int x, int l, int r) {
	if(l == r) return ;
	ac.init();
	for(int i = 0; i < SZ(str[x]); ++i)
		ac.add(q[str[x][i]].s);
	ac.build();
	for(int i = l; i <= r; ++i)
		if(q[i].t == 3) ans[i] += ac.query(q[i].s);
	int mid = (l + r) >> 1;
	dfs(x << 1, l, mid), dfs(x << 1 | 1, mid + 1, r);
}

int main() {
	int m;
	read(m);
	for(int i = 1; i <= m; ++i) {
		q[i].input();
		if(q[i].t == 1) id[gethash(q[i].s)] = i, ans[i] = -1;
		else if(q[i].t == 2) {
			int h = gethash(q[i].s), x = id[h];
			ins(1, 1, m, x, i, x), id.erase(h), ans[i] = -1;
		}
	}
	while(SZ(id)) {
		int x = id.begin()->se;
		ins(1, 1, m, x, m, x), id.erase(id.begin());
	}
	dfs(1, 1, m);
	for(int i = 1; i <= m; ++i)
		if(ans[i] != -1) printf("%d\n", ans[i]);
	return 0;
}
