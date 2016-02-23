#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
 
#define db double
#define ll long long
 
const int N = 1500;
const db eps = 1e-20;
 
#define sqr(x) (x*x)
 
int n;
struct pot {
    ll x, y;
    pot() {x = y = 0;}
    pot(ll _x, ll _y):x(_x), y(_y) {}
    inline void input() {
        scanf("%lld%lld", &x, &y);
    }
    inline friend pot operator + (const pot &p, const pot &q) {
        return pot(p.x+q.x, p.y+q.y);
    }
    inline friend pot operator - (const pot &p, const pot &q) {
        return pot(p.x-q.x, p.y-q.y);
    }
    inline friend ll operator ^ (const pot &p, const pot &q) {
        return p.x*q.y-p.y*q.x;
    }
    inline friend bool operator < (const pot &p, const pot &q) {
        return p.x < q.x || (p.x == q.x && p.y < q.y);
    }
    inline ll len2() {
        return sqr(x)+sqr(y);
    }
}p[N+9];
 
template<class T>
inline T abs(T x) {
    return x < 0 ? -x : x;
}
 
inline bool equal(db a, db b) {
    return abs(a-b) <= eps;
}
 
struct line {
    ll l2;
    pot a, v;
    line() {}
    line(pot _a, pot _b):a(_a) {v = _b-_a, l2 = v.len2();}
    inline friend bool cmp(const line &p, const line &q) {
        pot v = p.v;
        v = pot(v.y, -v.x);
        if(v.y == 0) return p.a.y < q.a.y || (p.a.y == q.a.y && p.a.x < q.a.x);
        else {
            db px = (p.a.x*v.y-p.a.y*v.x)/(db)v.y, qx = (q.a.x*v.y-q.a.y*v.x)/(db)v.y;
            return px < qx && !equal(px, qx);
        }
    }
    inline friend bool operator < (const line &p, const line &q) {
        return p.v.y*q.v.x < p.v.x*q.v.y || (p.v.y*q.v.x == p.v.x*q.v.y && p.l2 < q.l2) || (p.v.y*q.v.x == p.v.x*q.v.y && p.l2 == q.l2 && cmp(p, q));
    }
    inline friend bool operator == (const line &p, const line &q) {
        return p.v.y*q.v.x == p.v.x*q.v.y && p.l2 == q.l2 && !cmp(p, q) && !cmp(q, p);
    }
}l[N*N+9];
int ln;
 
inline void init() {
    scanf("%d", &n);
    for(int i = 1; i <= n; ++i)
        p[i].input();
}
 
inline void solve() {
    std::sort(p+1, p+n+1);
    for(int i = 1; i <= n; ++i)
        for(int j = i+1; j <= n; ++j)
            l[++ln] = line(p[i], p[j]);
    std::sort(l+1, l+ln+1);
    ll ans = 0;
    for(int i = 1; i <= ln; ++i) {
        int j = i, tot = 0;
        p[++tot] = l[i].a;
        while(j < ln && l[j+1] == l[i])
            p[++tot] = l[++j].a;
        pot x = p[1], y = p[1];
        for(int k = 1; k <= tot; ++k) {
            x = std::max(x, p[k]);
            y = std::min(y, p[k]);
        }
        pot a = l[i].v, b = x-y;
        ans = std::max(ans, abs(a^b));
        i = j;
    }
    std::cout << ans << std::endl;
}
 
int main() {
    init();
    solve();
    return 0;
}
