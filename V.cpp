#include <bits/stdc++.h>

#define ll long long

#define SZ(x) (int)(x).size()
#define pb push_back

template<class T>inline void chkmax(T &x, const T &y) {if(x < y) x = y;}
template<class T>inline void chkmin(T &x, const T &y) {if(x > y) x = y;}

template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x= x*10+c-'0';
    x *= f;
}
int outn;
char out[(int)2e7];
template<class T>
inline void write(T x) {
    if(x < 0) out[outn++] = '-', x = -x;
    if(x) {
        static int tmpn;
        static char tmp[20];
        tmpn = 0;
        while(x) tmp[tmpn++] = x%10+'0', x /= 10;
        while(tmpn) out[outn++] = tmp[--tmpn];
    }
    else out[outn++] = '0';
}

const int N = 5e5;
const ll INF = 5e14;

int n, m;
int a[N+9];

struct node {
    ll x, y, xmax, yxmax, v;
    node() {x = y = xmax = yxmax = v = 0;}
    node(ll _x, ll _y, ll _xmax, ll _yxmax):x(_x), y(_y), xmax(_xmax), yxmax(_yxmax) {v = 0;}
    inline void merge(const node &p) {
        chkmax(xmax, x+p.xmax), chkmax(yxmax, std::max(p.yxmax, v+p.xmax));
        v = std::max(v+p.x, p.y), x = std::max(-INF, x+p.x), y = std::max(p.x+y, p.y);
    }
}tr[N*4+9];

inline void pushdown(int x) {
    tr[x<<1].merge(tr[x]);
    tr[x<<1|1].merge(tr[x]);
    tr[x] = node(0, 0, 0, 0);
}

void mdf(int x, int l, int r, int ql, int qr, const node &p) {
    if(ql <= l && r <= qr) {
        tr[x].merge(p);
        return ;
    }
    pushdown(x);
    int mid = (l+r)>>1;
    if(ql <= mid) mdf(x<<1, l, mid, ql, qr, p);
    if(qr > mid) mdf(x<<1|1, mid+1, r, ql, qr, p);
}

ll qry(int x, int l, int r, int p, int t) {
    if(l == r) return t?std::max(a[p]+tr[x].xmax, tr[x].yxmax):std::max(a[p]+tr[x].x, tr[x].y);
    pushdown(x);int mid = (l+r)>>1;
    if(p <= mid) return qry(x<<1, l, mid, p, t);
    else return qry(x<<1|1, mid+1, r, p, t);
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("164.in", "r", stdin);
    freopen("164.out", "w", stdout);
#endif

    read(n), read(m);
    for(int i = 1; i <= n; ++i)
        read(a[i]);
    //printf("%lld %lld %lld %lld %lld\n", tr[1].v, tr[1].x, tr[1].y, tr[1].xmax, tr[1].yxmax);
    while(m--) {
        int t;
        read(t);
        if(t <= 3) {
            int l, r, x;
            read(l), read(r), read(x);
            if(t == 1) mdf(1, 1, n, l, r, node(x, 0, x, 0));
            else if(t == 2) mdf(1, 1, n, l, r, node(-x, 0, -x, 0));
            else mdf(1, 1, n, l, r, node(-INF, x, -INF, x));
        }
        else {
            int y;
            read(y);
            if(t == 4) write(qry(1, 1, n, y, 0));
            else write(qry(1, 1, n, y, 1));
            out[outn++] = '\n';
        }
        //printf("%lld %lld %lld %lld %lld\n", tr[1].v, tr[1].x, tr[1].y, tr[1].xmax, tr[1].yxmax);
    }
    printf("%s", out);
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
