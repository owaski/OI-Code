#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <climits>

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

const int N = 1e6, M = 1e6;
const int INF = INT_MAX;

int n, m, a[N+9];
struct data {
    int max, num, max2, tag;ll sum;
    data() {max = max2 = -1, num = 0, tag = INF, sum = 0;}
    data(int _max, int _num, int _max2, int _tag, ll _sum):max(_max), num(_num), max2(_max2), tag(_tag), sum(_sum) {}
}tr[N*4+9];

inline void pushup(int x) {
    data a = tr[x<<1], b = tr[x<<1|1], &c = tr[x];
    c.max = std::max(a.max, b.max), c.max2 = -1, c.num = 0;
    if(a.max == c.max) {
        c.num += a.num;
        chkmax(c.max2, a.max2);
    }
    else chkmax(c.max2, a.max);
    if(b.max == c.max) {
        c.num += b.num;
        chkmax(c.max2, b.max2);
    }
    else chkmax(c.max2, b.max);
    c.sum = a.sum+b.sum;
}

void mdf(int x, int l, int r, int v) {
    if(v >= tr[x].max) return ;
    if(v <= tr[x].max && v > tr[x].max2) {
        tr[x].sum -= 1ll*(tr[x].max-v)*tr[x].num;
        tr[x].max = v, tr[x].tag = v;
    }
    else if(v <= tr[x].max2) {
        if(l == r) {
            tr[x] = data(v, 1, -1, INF, v);
            return ;
        }
        int mid = (l+r)>>1;
        mdf(x<<1, l, mid, v);
        mdf(x<<1|1, mid+1, r, v);
        pushup(x);
    }
}

inline void pushdown(int x, int l, int r) {
    if(tr[x].tag < INF) {
        int mid = (l+r)>>1;
        mdf(x<<1, l, mid, tr[x].tag);
        mdf(x<<1|1, mid+1, r, tr[x].tag);
        tr[x].tag = INF;
    }
}

void build(int x, int l, int r) {
    if(l == r) {
        tr[x] = data(a[l], 1, -1, INF, a[l]);
        return ;
    }
    tr[x] = data(-1, 0, -1, INF, 0);
    int mid = (l+r)>>1;
    build(x<<1, l, mid);
    build(x<<1|1, mid+1, r);
    pushup(x);
}

void mdf(int x, int l, int r, int ql, int qr, int v) {
    if(ql <= l && r <= qr) {
        mdf(x, l, r, v);
        return ;
    }
    pushdown(x, l, r);
    int mid = (l+r)>>1;
    if(ql <= mid) mdf(x<<1, l, mid, ql, qr, v);
    if(qr > mid) mdf(x<<1|1, mid+1, r, ql, qr, v);
    pushup(x);
}

int qrymax(int x, int l, int r, int ql, int qr) {
    if(ql <= l && r <= qr) return tr[x].max;
    pushdown(x, l, r);int mid = (l+r)>>1, ret = 0;
    if(ql <= mid) chkmax(ret, qrymax(x<<1, l, mid, ql, qr));
    if(qr > mid) chkmax(ret, qrymax(x<<1|1, mid+1, r, ql, qr));
    return ret;
}

ll qrysum(int x, int l, int r, int ql, int qr) {
    if(ql <= l && r <= qr) return tr[x].sum;
    pushdown(x, l, r);int mid = (l+r)>>1;ll ret = 0;
    if(ql <= mid) ret += qrysum(x<<1, l, mid, ql, qr);
    if(qr > mid) ret += qrysum(x<<1|1, mid+1, r, ql, qr);
    return ret;
}

int main() {
    int Case;
    read(Case);
    while(Case--) {
        read(n), read(m);
        for(int i = 1; i <= n; ++i)
            read(a[i]);
        build(1, 1, n);
        while(m--) {
            int type, l, r;
            read(type), read(l), read(r);
            if(type == 0) {
                int t;
                read(t);
                mdf(1, 1, n, l, r, t);
            }
            else if(type == 1) {
                write(qrymax(1, 1, n, l, r));
                out[outn++] = '\n';
            }
            else {
                write(qrysum(1, 1, n, l, r));
                out[outn++] = '\n';
            }
        }
    }
    printf("%s", out);
    return 0;
}
