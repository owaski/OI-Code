#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <cmath>

#define pii std::pair<int,int>
#define mp std::make_pair
#define fi first
#define se second

#define SZ(x) ((int)(x).size())
#define pb push_back

#define ll long long
#define debug(...) fprintf(stderr, __VA_ARGS__)

template<class T>inline void chkmin(T &x, const T &y) {if(x > y) x = y;}
template<class T>inline void chkmax(T &x, const T &y) {if(x < y) x = y;}

template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c = getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c - '0';
    while((c = getchar()) >= '0' && c <= '9') x = x * 10 + c - '0';
    x *= f;
}
char out[(int)1e7];
int outn;
template<class T>
inline void write(T x) {
    if(x < 0) out[outn++] = '-', x = -x;
    if(x) {
        static char t[20];
        static int n;n = 0;
        while(x) t[n++] = x % 10 + '0', x /= 10;
        while(n) out[outn++] = t[--n];
    }
    else out[outn++] = '0';
}

const int N = 2e5;
const int S = 20000000;
const double eps = 1e-8;

int n, m;
int a[N + 9];
pii v[N + 9];
double lg[N + 9];

namespace seg {
    struct data {
        int lc, rc, size;
        double sum;
        void init() {
            size = sum = 0;
        }
    }tr[S + 9];
    int trn;
	
#define LC(x) (tr[x].lc)
#define RC(x) (tr[x].rc)
#define S(x) (tr[x].sum)
#define SS(x) (tr[x].size)	
	
    void pushup(int x) {
        S(x) = S(LC(x)) + S(RC(x));
        SS(x) = SS(LC(x)) + SS(RC(x));
    }
	
    int newnode(int lc = 0, int rc = 0) {
        tr[++trn].init();
        tr[trn].lc = lc, tr[trn].rc = rc;
        pushup(trn);
        return trn;
    }
    
    int ins(int l, int r, int p) {
        int x = newnode();
        S(x) = lg[p], SS(x) = 1;
        if(l == r) return x;
        int mid = (l + r) >> 1;
        if(p <= mid) LC(x) = ins(l, mid, p);
        else RC(x) = ins(mid + 1, r, p);
        return x;
    }
	
    int splitl(int x, int l, int r, int k) {
        if(SS(x) <= k) return x;
        if(!k) return 0;
        int mid = (l + r) >> 1;
        if(SS(LC(x)) >= k) return newnode(splitl(LC(x), l, mid, k), 0);
        else return newnode(LC(x), splitl(RC(x), mid + 1, r, k - SS(LC(x))));
    }

    int splitr(int x, int l, int r, int k) {
        if(!k) return x;
        if(SS(x) <= k) return 0;
        int mid = (l + r) >> 1;
        if(SS(LC(x)) <= k) return newnode(0, splitr(RC(x), mid + 1, r, k - SS(LC(x))));
        else return newnode(splitr(LC(x), l, mid, k), RC(x));
    }

    int merge(int x, int y, int l, int r) {
        if(!x || !y) return x + y;
        S(x) += S(y), SS(x) += SS(y);
        if(l == r) return x;
        int mid = (l + r) >> 1;
        LC(x) = merge(LC(x), LC(y), l, mid);
        RC(x) = merge(RC(x), RC(y), mid + 1, r);
        return x;
    }
}

namespace bit {
    double sum[N + 9];
#define lowbit(x) ((x) & -(x))
    void add(int x, double v) {
        for(; x <= n; x += lowbit(x)) sum[x] += v;
    }
    double qry(int x) {
        double ret = 0;
        for(; x; x -= lowbit(x)) ret += sum[x];
        return ret;
    }
}

std::set<int> s;
int rt[N + 9];
int fl[N + 9];

void debugset() {    
    for(std::set<int>::iterator i = s.begin(); i != s.end(); ++i)
        printf("(%d,%d,%d) ", *i, fl[*i], seg::tr[rt[*i]].size);
    puts("");
}

void split(int a, int b) {
    if(a == b) return ;
    if(b > a + seg::tr[rt[a]].size - 1) return ;
    if(fl[a]) {
        rt[b] = seg::splitr(rt[a], 1, n, b - a);
        bit::add(b, seg::tr[rt[b]].sum);
        bit::add(a, -seg::tr[rt[b]].sum);
        rt[a] = seg::splitl(rt[a], 1, n, b - a);
    }
    else {
        rt[b] = seg::splitl(rt[a], 1, n, seg::tr[rt[a]].size - (b - a));
        bit::add(b, seg::tr[rt[b]].sum);
        bit::add(a, -seg::tr[rt[b]].sum);
        rt[a] = seg::splitr(rt[a], 1, n, seg::tr[rt[a]].size - (b - a));
    }
    fl[b] = fl[a], s.insert(b);
}

int pos(int x) {
    std::set<int>::iterator t = s.lower_bound(x);
    if(*t == x) return x;
    else return *(--t);
}

void getinterval(int l, int r) {
    int x = pos(l), y = pos(r);
    if(x == y) split(x, l), split(l, r + 1);
    else split(x, l), split(y, r + 1);
}

void sort(int l, int r, int flag) {
    if(l == r) return ;
    getinterval(l, r);
    static int seq[N + 9], seqn;
    seqn = 0;
    for(std::set<int>::iterator i = s.upper_bound(l); *i <= r; ++i)
        seq[++seqn] = *i;
    for(int i = 1; i <= seqn; ++i) {
        bit::add(seq[i], -seg::tr[rt[seq[i]]].sum);
        bit::add(l, seg::tr[rt[seq[i]]].sum);
        s.erase(seq[i]);
        seg::merge(rt[l], rt[seq[i]], 1, n);
        rt[seq[i]] = fl[seq[i]] = 0;
    }
    fl[l] = flag;
}

int qry(int l, int r) {
    getinterval(l, r);
    double sum = bit::qry(r) - bit::qry(l - 1);
    sum -= (int)sum;
    return (int)(pow(10, sum) + eps);
}

int main() {
    freopen("Philosopher.in", "r", stdin);
    freopen("Philosopher.out", "w", stdout);

    read(n), read(m);
    for(int i = 1; i <= n; ++i) read(a[i]), v[i] = mp(a[i], i);
    std::sort(v + 1, v + n + 1);
    for(int i = 1; i <= n; ++i) lg[i] = log10(v[i].fi), a[v[i].se] = i;
    s.insert(0), s.insert(n + 1);
    for(int i = 1; i <= n; ++i) s.insert(i), rt[i] = seg::ins(1, n, a[i]), bit::add(i, lg[a[i]]);
    while(m--) {
        int type, l, r, flag;
        read(type), read(l), read(r);
        if(type == 1) read(flag), sort(l, r, flag);
        else printf("%d\n", qry(l, r));
    }
    
    fclose(stdin);fclose(stdout);
    return 0;
}
