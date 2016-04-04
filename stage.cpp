#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>

#define double long double

const int N = 1000, M = 1000;

int n, m;

struct pot {
    double x, y;
    pot() {x = y = 0;}
    pot(double _x, double _y):x(_x), y(_y) {}
    inline void input() {
        scanf("%Lf%Lf", &x, &y);
    }
    inline friend bool operator < (const pot &p, const pot &q) {
        return p.x < q.x || (p.x == q.x && p.y < q.y);
    }
}a[N+9];

struct data {
    double x, y, p, a;
    data() {x = y = p = a = 0;}
    data(double _x, double _y, double _p):x(_x), y(_y), p(_p) {a = atan2(y, x);}
    inline void input() {
        scanf("%Lf%Lf%Lf", &x, &y, &p);
    }
    inline friend bool operator < (const data &p, const data &q) {
        return p.a < q.a;
    }
}b[M+9], u[M+9], v[M+9];

double pu[N*4+9], pv[N*4+9];

void maketree(int x, int l, int r, data *g, double *p) {
    if(l > r) {
        p[x] = 1;
        return ;
    }
    if(l == r) {
        p[x] = 1-g[l].p;
        return ;
    }
    int mid = (l+r)>>1;
    maketree(x<<1, l, mid, g, p);
    maketree(x<<1|1, mid+1, r, g, p);
    p[x] = p[x<<1]*p[x<<1|1];
}

double qry(int x, int l, int r, int tl, int tr, double *p) {
    if(tl > tr) return 1;
    if(l == tl && r == tr) return p[x];
    int mid = (l+r)>>1;
    if(tr <= mid) return qry(x<<1, l, mid, tl, tr, p);
    else if(tl > mid) return qry(x<<1|1, mid+1, r, tl, tr, p);
    else return qry(x<<1, l, mid, tl, mid, p)*qry(x<<1|1, mid+1, r, mid+1, tr, p);
}

int main() {
    freopen("stage.in", "r", stdin);
    freopen("stage.out", "w", stdout);

    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; ++i)
        a[i].input();
    for(int i = 1; i <= m; ++i)
        b[i].input();
    std::sort(a+1, a+n+1);
    double ans = n;
    for(int i = 1; i <= n; ++i) {
        int un = 0, vn = 0;
        for(int j = 1; j <= m; ++j)
            if(b[j].x <= a[i].x) u[++un] = data(a[i].x-b[j].x, a[i].y-b[j].y, b[j].p);
            else v[++vn] = data(b[j].x-a[i].x, b[j].y-a[i].y, b[j].p);
        std::sort(u+1, u+un+1), std::sort(v+1, v+vn+1);
        maketree(1, 1, un, u, pu), maketree(1, 1, vn, v, pv);
        ans -= pu[1]*pv[1];
        for(int j = 1, k = 0; j <= un; ++j) {
            while(k < vn && v[k+1].a < u[j].a) ++k;
            ans -= u[j].p*qry(1, 1, un, j+1, un, pu)*qry(1, 1, vn, 1, k, pv);
        }
        for(int j = 1, k = 0; j <= vn; ++j) {
            while(k < un && u[k+1].a < v[j].a) ++k;
            ans -= v[j].p*qry(1, 1, vn, j+1, vn, pv)*qry(1, 1, un, 1, k, pu);
        }
    }
    printf("%.9Lf\n", ans);
    
    fclose(stdin);fclose(stdout);
    return 0;
}
