#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>

#define db double

const int N = 10000;
const db eps = 1e-10;

db a, b, vmax, f;
int n;
struct path {
    db l, s, minspeed;
    path() {l = s = 0;}
    path(db _l, db _s):l(_l), s(_s) {minspeed = std::min(vmax, std::max(0.0, -b*s/a));}
    inline void input() {
        db x, y;
        scanf("%lf%lf", &x, &y);
        *this = path(sqrt(x*x+y*y)/1000, y/x);
    }
    inline friend bool operator < (const path &p, const path &q) {
        return p.minspeed > q.minspeed;
    }
}p[N+9];
db ssum[N+9];
db lsum[N+9];

inline void init() {
    std::cin >> a >> b >> vmax >> f >> n;
    for(int i = 1; i <= n; ++i) p[i].input();
}

inline void solve() {
    std::sort(p+1, p+n+1);
    ssum[n+1] = lsum[n+1] = 0;
    for(int i = n; i >= 1; --i) {
        ssum[i] = ssum[i+1]+b*p[i].l*p[i].s;
        lsum[i] = lsum[i+1]+p[i].l;
    }
    db ans = 0;
    int start = 1;
    while(start <= n && p[start].minspeed > vmax-eps)
        ans += p[start++].l/vmax;
    for(int i = start; i <= n; ++i) {
        int j = i;
        db tmp = f-ssum[i];
        if(tmp < 0) {
            puts("IMPOSSIBLE");
            return ;
        }
        db curspeed = std::min(vmax, tmp/(a*lsum[i]));
        if(curspeed >= p[j].minspeed) {
            ans += lsum[i]/curspeed;
            break;
        }
        else {
            while(j <= n && curspeed < p[j].minspeed) {
                ans += p[j].l/p[j].minspeed;
                j++;
            }
            i = j-1;
        }
    }
    printf("%.5lf\n", ans);
}

int main() {
    int Test;
    scanf("%d", &Test);
    while(Test--) {
        init();
        solve();
    }
    return 0;
}
