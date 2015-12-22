#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)

template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}

const int N = 200009;
const int INF = 1e9;

int n, m;

struct Tree {
    int sz, head[N], to[N<<1], ne[N<<1];

    Tree() {
        sz = 1;
        memset(head, 0, sizeof head);
    }

    inline void add_edge(int u, int v) {
        to[sz] = v, ne[sz] = head[u], head[u] = sz++;
        to[sz] = u, ne[sz] = head[v], head[v] = sz++;
    }
}T;

struct Tag {
    int mult, add;//mult*x+add

    Tag() {mult = 1, add = 0;}
    Tag(int _mult, int _add):mult(_mult), add(_add) {}

    inline void init() {
        mult = 1, add = 0;
    }

    inline bool empty() {
        return mult == 1 && add == 0;
    }
};

inline Tag operator + (const Tag &p, const Tag &q) {
    return Tag(q.mult*p.mult, q.mult*p.add+q.add);//q operate on p
}

struct Info {
    int min, max, sum, size;

    Info() {min = INF, max = -INF, sum = 0, size = 0;}
    Info(int _min, int _max, int _sum, int _size):min(_min), max(_max), sum(_sum), size(_size) {}

    inline void init() {
        min = INF, max = -INF, sum = 0, size = 0;
    }
};

inline Info operator + (const Info &a, const Info &b) {
    return Info(std::min(a.min, b.min), std::max(a.max, b.max), a.sum+b.sum, a.size+b.size);//merge a and b
}

inline int opt(const int &a, const Tag &b) {
    return a*b.mult+b.add;//calc mult*x+add
}

inline Info operator + (const Info &a, const Tag &b) {//empty node can't be operated, or the INF will be changed
    return a.size?Info(opt(a.min, b), opt(a.max, b), b.mult*a.sum+b.add*a.size, a.size):a;//operate b on a
}

int fa[N], c[N][4], val[N];
Info tree[N], chain[N], all[N];
Tag tag_tree[N], tag_chain[N];
int pool[N], pool_top;
bool inner[N], rev[N];
int root;

inline void init_node(int x) {
    tree[x].init(), chain[x].init(), all[x].init();
    tag_tree[x].init(), tag_chain[x].init();
}

inline int new_node() {
    int ret = pool[pool_top--];
    init_node(ret), inner[ret] = true, rev[ret] = false;
    return ret;
}

inline void del_node(int x) {
    pool[++pool_top] = x;
}

inline void setc(int x, int y, int p) {
    if(y) fa[y] = x;c[x][p] = y;
}   

inline void push_up(int x) {
    chain[x].init(), tree[x].init(), all[x].init();
    if(!inner[x]) chain[x] = all[x] = Info(val[x], val[x], val[x], 1);
    for(int i = 0; i < 2; ++i) {
        chain[x] = chain[x]+chain[c[x][i]];
        tree[x] = tree[x]+tree[c[x][i]];
    }
    for(int i = 2; i < 4; ++i) tree[x] = tree[x]+all[c[x][i]];
    for(int i = 0; i < 4; ++i) all[x] = all[x]+all[c[x][i]];
}

inline void update_chain_tree(int x, const Tag &p) {
    tree[x] = tree[x]+p, all[x] = all[x]+p, tag_tree[x] = tag_tree[x]+p;
}

inline void update_tree(int x, const Tag &p) {
    tree[x] = tree[x]+p, tag_tree[x] = tag_tree[x]+p;
    chain[x] = chain[x]+p, tag_chain[x] = tag_chain[x]+p;
    if(!inner[x]) val[x] = opt(val[x], p);
    all[x] = tree[x]+chain[x];
}

inline void update_chain_chain(int x, const Tag &p) {
    chain[x] = chain[x]+p, all[x] = chain[x]+tree[x], tag_chain[x] = tag_chain[x]+p, val[x] = opt(val[x], p);
}

inline void push_down(int x) {
    if(rev[x]) {
        for(int i = 0; i < 2; ++i)
            if(c[x][i])
                rev[c[x][i]] ^= true;
        std::swap(c[x][0], c[x][1]);
        rev[x] = false;
    }
    if(!tag_tree[x].empty()) {
        for(int i = 0; i < 2; ++i) if(c[x][i]) update_chain_tree(c[x][i], tag_tree[x]);
        for(int i = 2; i < 4; ++i) if(c[x][i]) update_tree(c[x][i], tag_tree[x]);
        tag_tree[x].init();
    }
    if(!tag_chain[x].empty()) {
        for(int i = 0; i < 2; ++i) if(c[x][i]) update_chain_chain(c[x][i], tag_chain[x]);
        tag_chain[x].init();
    }
}

inline int pos(int x) {
    for(int i = 0; i < 4; ++i)
        if(c[fa[x]][i] == x)
            return i;
    return 4;
}

inline int C(int x, int p) {
    if(c[x][p]) push_down(c[x][p]);
    return c[x][p];
}

//tp = 0, splay in main tree; while 2, splay in extra tree

inline bool d(int x, int tp) {
    return c[fa[x]][tp+1] == x;
}

inline bool is_root(int x, int tp) {
    if(tp == 0) return fa[x] == 0 || (c[fa[x]][0] != x && c[fa[x]][1] != x);
    else return fa[x] == 0 || !inner[fa[x]] || !inner[x];
}

inline void rotate(int x, int tp) {
    int f = fa[x], p = d(x, tp);
    if(fa[f]) setc(fa[f], x, pos(f));else fa[x] = 0;
    setc(f, c[x][!p+tp], p+tp), setc(x, f, !p+tp), push_up(f);
}

inline void splay(int x, int tp) {
    static int anc[N], n;anc[n = 1] = x;
    for(int i = x; fa[i]; i = fa[i]) anc[++n] = fa[i];
    for(int i = n; i >= 1; --i) push_down(anc[i]);
    while(!is_root(x, tp)) {
        if(!is_root(fa[x], tp)) {
            if(d(x, tp) == d(fa[x], tp)) rotate(fa[x], tp);
            else rotate(x, tp);
        }
        rotate(x, tp);
    }
    push_up(x);
}

inline void del(int x) {
    if(inner[fa[x]]) {
        int f = fa[x], ff = fa[f];
        setc(ff, c[f][pos(x)^1], pos(f));
        del_node(f), splay(ff, 2);
    }
    else setc(fa[x], 0, pos(x));
    fa[x] = 0;
}

inline void add(int x, int y) {//x->y
    push_down(x);
    for(int i = 2; i < 4; ++i)
        if(!c[x][i]) {
            setc(x, y, i);
            return ;
        }
    while(inner[c[x][2]]) x = C(x, 2);
    int z = new_node();
    setc(z, c[x][2], 2), setc(z, y, 3);
    setc(x, z, 2), splay(z, 2);
}

inline int access(int x) {
    int p, q;
    for(p = x, q = 0; p;) {
        splay(p, 0);
        if(c[p][1]) add(p, c[p][1]);
        if(q) push_down(fa[q]), del(q);
        setc(p, q, 1), push_up(p);
        q = p, p = fa[p], n = 0;
        while(inner[p]) p = fa[p];
        if(inner[fa[q]]) splay(fa[q], 2);
    }
    return q;
}

inline void make_root(int x) {
    access(x), splay(x, 0), rev[x] ^= true;
}

inline void link(int u, int v) {//u->v
    make_root(u), make_root(v), add(u, v), push_up(u);
}

inline int find_father(int u) {
    int ret;
    access(u), splay(u, 0), ret = C(u, 0);
    while(c[ret][1]) ret = C(ret, 1);
    return ret;
}

inline void cut(int u) {
    int v = find_father(u);
    if(v) {
        access(v);
        del(u), push_up(v);
    }
}

inline int LCA(int u, int v) {
    access(u);
    return access(v);
}

inline void init() {
    read(n), read(m);
    for(int i = 1; i < n; ++i) {
        int u, v;
        read(u), read(v);
        //debug("(%d,%d)\n", u, v);
        T.add_edge(u, v);
    }
    for(int i = 1; i <= n; ++i) {
        read(val[i]);
        push_up(i);
    }
    read(root);
    for(int i = 1; i <= n; ++i)
        pool[++pool_top] = 2*n-i+1;
}

inline void bfs() {
    static bool vis[N];
    static int que[N], l, r;
    que[++r] = root, vis[root] = true;
    while(l < r) {
        int now = que[++l];
        //debug("%d\n", now);
        for(int i = T.head[now]; i; i = T.ne[i])
            if(!vis[T.to[i]]) {                
                vis[T.to[i]] = true;
                link(now, T.to[i]), que[++r] = T.to[i];
            }
    }
    make_root(root);
    //for(int i = 1; i <= n; ++i)
        //debug("father[%d] = %d\n", i, father[i]);
}

inline void solve() {
    bfs();
    int ans;  
    while(m--) {
        int tp, x, y, z;
        read(tp), read(x);
        if(tp == 0 || tp == 3 || tp == 4 || tp == 5 || tp == 11) {
            access(x);
            if(tp == 0 || tp == 5) {
                read(y);
                Tag t = Tag(tp==5, y);
                val[x] = opt(val[x], t);
                for(int i = 2; i < 4; ++i)
                    if(c[x][i])
                        update_tree(c[x][i], t);
                push_up(x), access(x), splay(x, 0);
            }
            else {
                ans = val[x];
                for(int i = 2; i < 4; ++i)
                    if(c[x][i]) {
                        if(tp == 3) ans = std::min(ans, all[c[x][i]].min);
                        else if(tp == 4) ans = std::max(ans, all[c[x][i]].max);
                        else ans += all[c[x][i]].sum;
                    }
                printf("%d\n", ans);
            }
        }
        else if(tp == 2 || tp == 6 || tp == 7 || tp == 8 || tp == 10) {
            read(y);
            make_root(x);
            access(y);
            splay(y, 0);
            if(tp == 2 || tp == 6) {
                read(z);
                Tag t = Tag(tp==6, z);
                update_chain_chain(y, t);
            }
            else {
                if(tp == 7) ans = chain[y].min;
                else if(tp == 8) ans = chain[y].max;
                else ans = chain[y].sum;
                printf("%d\n", ans);
            }
            make_root(root);
        }
        else if(tp == 1) make_root(root = x);
        else {
            read(y);
            if(LCA(x, y) == x)
                continue;
            cut(x), link(y, x), make_root(root);
        }
    }
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
