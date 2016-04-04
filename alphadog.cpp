#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

#define ll long long

template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}

int outn;
char out[10000000];
template<class T>
inline void write(T x) {
    if(x < 0) out[outn++] = '-', x = -x;
    if(x) {
        int tmpn = 0;
        char tmp[20] = {0};
        while(x) tmp[tmpn++] = x%10+'0', x /= 10;
        while(tmpn) out[outn++] = tmp[--tmpn];
    }
    else out[outn++] = '0';
}

const int N = 1e5;
const int SIGMA = 26;

int n, sig;

int id[N+9];

namespace lct {
    struct data {
        int lfa, l, x;
        ll s, sfa, slfa, sl;
        data() {lfa = l = x = s = sfa = sl = slfa = 0;}
        data(int _lfa, int _l):lfa(_lfa), l(_l) {x = s = sfa = 0, sl = l, slfa = lfa;}
    }p[N+9];
    int fa[N+9], c[N+9][2], add[N+9];
    int pn;
    inline int newnode(int lfa, int l) {
        int ret = ++pn;
        p[ret] = data(std::max(lfa, 0), l);
        return ret;
    }
    inline bool d(int x) {
        return c[fa[x]][1] == x;
    }
    inline void pushup(int x) {
        p[x].sl = p[x].l;
        p[x].slfa = p[x].lfa;
        p[x].s = 1ll*p[x].l*p[x].x;
        p[x].sfa = 1ll*p[x].lfa*p[x].x;
        for(int i = 0; i < 2; ++i) {
            p[x].s += p[c[x][i]].s;
            p[x].sfa += p[c[x][i]].sfa;
            p[x].sl += p[c[x][i]].sl;
            p[x].slfa += p[c[x][i]].slfa;
        }
    }
    inline void updadd(int x, int y) {
        add[x] += y, p[x].x += y, p[x].s += p[x].sl*y, p[x].sfa += p[x].slfa*y;
    }
    inline void pushdown(int x) {
        if(add[x]) {
            for(int i = 0; i < 2; ++i)
                if(c[x][i]) updadd(c[x][i], add[x]);
            add[x] = 0;
        }
    }
    inline void setc(int x, int y, int d) {
        c[x][d] = y;if(y) fa[y] = x;
    }
    inline bool root(int x) {
        return !fa[x] || (c[fa[x]][0] != x && c[fa[x]][1] != x);
    }
    inline void rot(int x) {
        int f = fa[x], p = d(x);
        if(root(f)) fa[x] = fa[f];
        else setc(fa[f], x, d(f));
        setc(f, c[x][p^1], p), setc(x, f, p^1);
        pushup(f);
    }
    inline void splay(int x) {
        static int anc[N+9], n;
        anc[n = 1] = x;
        for(int i = x; fa[i]; i = fa[i])
            anc[++n] = fa[i];
        while(n) pushdown(anc[n--]);
        while(!root(x)) {
            if(!root(fa[x])) {
                if(d(x) == d(fa[x])) rot(fa[x]);
                else rot(x);
            }
            rot(x);
        }
        pushup(x);
    }
    inline void access(int x) {
        for(int y = 0; x; y = x, x = fa[x])
            splay(x), setc(x, y, 1), pushup(x);
    }
    inline void link(int x, int y) {
        fa[y] = x;
    }
    inline void mdf(int x) {
        access(x), splay(x), updadd(x, 1);
    }
    inline ll calc(int x) {
        access(x), splay(x);
        return p[x].s-p[x].sfa;
    }
}

namespace pam {
    int fa[N+9], tr[N+9][SIGMA], len[N+9];
    int sz, lst, all, str[N+9];
    inline int newnode(int l) {
        int ret = sz++;
        memset(tr[ret], 0, sizeof tr[ret]);
        len[ret] = l;
        return ret;
    }
    inline void init() {
        memset(str, -1, sizeof str);
        int x = newnode(0), y = newnode(-1);
        fa[x] = y, lst = x, all = 0;
    }
    inline int find(int x) {
        while(str[all-1-len[x]] != str[all]) x = fa[x];
        return x;
    }
    inline ll add(int x) {
        str[++all] = x;
        int p = find(lst), r;
        if(!tr[p][x]) {
            r = newnode(len[p]+2);
            fa[r] = tr[find(fa[p])][x];
            tr[p][x] = r;
            id[r] = lct::newnode(len[fa[r]], len[r]);
            if(id[fa[r]]) lct::link(id[fa[r]], id[r]);
        }
        else r = tr[p][x];
        lct::mdf(id[r]);
        lst = r;
        return lct::calc(id[r]);
    }
    inline void DEBUG() {
        for(int i = 1; i <= all; ++i)
            putchar(str[i]+'a');
        putchar('\n');
        for(int i = 0; i < sz; ++i)
            printf("%d : len = %d\n", i, len[i]);
    }
}

int main() {
    freopen("alphadog.in", "r", stdin);
    freopen("alphadog.out", "w", stdout);

    ll lstans = 0;
    read(n), read(sig);
    pam::init();
    while(n--) {
        ll x;read(x);
        if(sig) x ^= lstans;
        lstans += pam::add(x);
        write(lstans), out[outn++] = '\n';
    }
    printf("%s", out);
    
    fclose(stdin);fclose(stdout);
    return 0;
}
