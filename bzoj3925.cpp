#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>

#define ld __float128

const int N = 10, M = N*(N-1)/2;
const int S = 1<<N;

int n, m;

struct edge {
	int u, v;
	edge() {u = v = 0;}
	edge(int _u, int _v):u(_u), v(_v) {}
	inline void input() {
		scanf("%d%d", &u, &v);
	}
}e[M+9];

struct poly {
	int n;
	ld a[M+9];//a[0]+a[1]x+a[2]x^2+...+a[n-1]x^(n-1)
	poly() {
		n = 0, memset(a, 0, sizeof a);
	}
	inline void init() {
		n = 1, a[0] = 1;
	}
	inline friend poly operator * (const poly &p, const poly &q) {
		poly ret;
		ret.n = p.n+q.n-1;
		for(int i = 0; i < p.n; ++i)
			for(int j = 0; j < q.n; ++j)
				ret.a[i+j] += p.a[i]*q.a[j];
		return ret;	
	}
	inline friend poly operator - (const poly &p, const poly &q) {
		poly ret;
		ret.n = std::max(p.n, q.n);
		for(int i = 0; i < ret.n; ++i)
			ret.a[i] = p.a[i]-q.a[i];
		return ret;
	}
	inline void operator += (const poly &p) {
		n = std::max(n, p.n);
		for(int i = 0; i < n; ++i)
			a[i] += p.a[i];
	}
	inline ld integration() {
		ld ret = 0;
		for(int i = 0; i < n; ++i)
			ret += a[i]/(i+1);
		return ret;
	}
}f[S+9], p[M+9], mult;

inline bool in(int S, int x) {
    	return S & (1<<(x-1));
}

inline bool between(int S, int T, int u, int v) {
    	return (in(S, u) && in(T, v)) || (in(S, v) && in(T, u));
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	scanf("%d%d", &n, &m);
	for(int i = 1; i <= m; ++i)
		e[i].input();
	p[0].init();
	mult.n = 2, mult.a[0] = 1, mult.a[1] = -1;
	for(int i = 1; i <= m; ++i)
		p[i] = p[i-1]*mult;
	for(int i = 1; i < (1<<n); ++i) {
		int g = 0;
		for(int j = 1; j <= n; ++j)
			if(i&(1<<(j-1))) {
				g = j;
				break;
			}
		for(int j = i&(i-1); j; j = i&(j-1))
			if(j&(1<<(g-1))) {
				int cnt = 0;
				for(int k = 1; k <= m; ++k)
					if(between(j, i^j, e[k].u, e[k].v))
						cnt++;
				f[i] += (p[0]-f[j])*p[cnt];
			}
	}
    	printf("%.6lf\n", (double)f[(1<<n)-1].integration());

	fclose(stdin);fclose(stdout);
	return 0;
}
