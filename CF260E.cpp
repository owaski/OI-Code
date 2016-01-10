#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

#define debug(...) fprintf(stderr, __VA_ARGS__)

const int N = 1e5+9;
const int LOG = 20;

int n;
int a[10];
int b[10];
bool vis[10];

int cx[N], cy[N];
int x[N], y[N];
int vx[N], vxn;
int vy[N], vyn;

struct Pot {
    int x, y;
    Pot() {x = y = 0;}
    Pot(int _x, int _y):x(_x), y(_y) {}
    inline void input() {
        scanf("%d%d", &x, &y);
        vx[++vxn] = x, vy[++vyn] = y;
    }
}p[N];

struct Node {
    int c[2], size;
    Node() {
        c[0] = c[1] = size = 0;
    }
}node[N*LOG];
int tot, root[N];

inline void init() {
    scanf("%d", &n);
    for(int i = 1; i <= n; ++i)
        p[i].input();
    for(int i = 1; i <= 9; ++i)
        scanf("%d", a+i);
}

inline bool cmpx(const Pot &a, const Pot &b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

inline void discrete() {
    std::sort(vx+1, vx+vxn+1);
    vxn = std::unique(vx+1, vx+vxn+1)-vx-1;
    std::sort(vy+1, vy+vyn+1);
    vyn = std::unique(vy+1, vy+vyn+1)-vy-1;
    for(int i = 1; i <= n; ++i) {
        p[i].x = std::lower_bound(vx+1, vx+vxn+1, p[i].x)-vx;
        p[i].y = std::lower_bound(vy+1, vy+vyn+1, p[i].y)-vy;
        cx[p[i].x]++, cy[p[i].y]++, x[i] = p[i].x, y[i] = p[i].y;
    }
    std::sort(x+1, x+n+1), std::sort(y+1, y+n+1);
    for(int i = 1; i <= vxn; ++i) cx[i] += cx[i-1];
    for(int i = 1; i <= vyn; ++i) cy[i] += cy[i-1];
}

#define LC(x) node[x].c[0]
#define RC(x) node[x].c[1]

inline void update(int x) {
    node[x].size = node[LC(x)].size+node[RC(x)].size;
}

void insert(int &x, int y, int l, int r, int p) {
    x = ++tot;
    if(l == r) {
        node[x].size = node[y].size+1;
        return ;
    }
    int mid = (l+r)>>1;
    if(p <= mid) {
        RC(x) = RC(y);
        insert(LC(x), LC(y), l, mid, p);
    }
    else {
        LC(x) = LC(y);
        insert(RC(x), RC(y), mid+1, r, p);
    }
    update(x);
}

int calc(int x, int l, int r, int p) {
    if(r <= p) return node[x].size;
    int mid = (l+r)>>1;
    if(p <= mid) return calc(LC(x), l, mid, p);
    else return node[LC(x)].size+calc(RC(x), mid+1, r, p);
}

inline int qry(int a, int b) {
    a = std::upper_bound(x+1, x+n+1, a)-x-1;
    return calc(root[a], 1, vyn, b);
}

inline void work() {
    int s1 = b[1]+b[2]+b[3], s2 = s1+b[4]+b[5]+b[6];
    if(cx[x[s1]] != s1 || cx[x[s2]] != s2) return ;
    int x1 = x[s1], x2 = x[s2];
    s1 = b[1]+b[4]+b[7], s2 = s1+b[2]+b[5]+b[8];
    if(cy[y[s1]] != s1 || cy[y[s2]] != s2) return ;
    int y1 = y[s1], y2 = y[s2];
    if(qry(x1, y1) == b[1] && qry(x1, y2) == b[1]+b[2] && qry(x2, y1) == b[1]+b[4] && qry(x2, y2) == b[1]+b[2]+b[4]+b[5]) {
        printf("%lf %lf\n", vx[x1]+0.5, vx[x2]+0.5);
        printf("%lf %lf\n", vy[y1]+0.5, vy[y2]+0.5);
        exit(0);
    }
}

void dfs(int now) {
    if(now == 10) {
        work();
        return ;
    }
    for(int i = 1; i <= 9; ++i)
        if(!vis[i]) {
            vis[i] = true;
            b[now] = a[i];
            dfs(now+1);
            b[now] = 0;
            vis[i] = false;
        }
}

inline void solve() {
    discrete();
    std::sort(p+1, p+n+1, cmpx);
    for(int i = 1; i <= n; ++i)
        insert(root[i], root[i-1], 1, vyn, p[i].y);
    dfs(1);
    puts("-1");
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    init();
    solve();
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
