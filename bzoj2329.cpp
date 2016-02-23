#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
 
const int N = 100000, M = N;
const int INF = 1e9;
 
int n, m;
char s[N+9];
 
struct data {
    int sum, size;
    int premin, sufmin;
    int premax, sufmax;
    int ans;
    data() {sum = size = ans = 0, premin = sufmin = INF, premax = sufmax = -INF;}
    data(int _sum, int _size, int _premin, int _sufmin, int _premax, int _sufmax):sum(_sum), size(_size), premin(_premin), sufmin(_sufmin), premax(_premax), sufmax(_sufmax) {}
 
    inline void getans() {   
        if(size&1) ans = INF;
        else {
            if(premin < 0) ans = ((1-premin)>>1)+(((((1-premin)>>1)<<1)+sum)>>1);
            else ans = sum>>1;
        }
    }
};
 
inline data merge(const data &p, int c, const data &q) {
    data ret;
    ret.size = p.size+q.size+1;
    ret.sum = p.sum+c+q.sum;
    ret.premin = std::min(p.premin, p.sum+c+std::min(0, q.premin));
    ret.sufmin = std::min(q.sufmin, q.sum+c+std::min(0, p.sufmin));
    ret.premax = std::max(p.premax, p.sum+c+std::max(0, q.premax));
    ret.sufmax = std::max(q.sufmax, q.sum+c+std::max(0, p.sufmax));
    ret.getans();
    return ret;
}
 
struct tag {
    bool rev, inv;
    int rep;
    tag() {rev = inv = rep = 0;}
    tag(bool _rev, bool _inv, int _rep):rev(_rev), inv(_inv), rep(_rep) {}
};
 
struct node {
    int t;data v;tag g;
    node *fa, *c[2];
    node() {fa = c[0] = c[1] = 0, t = 0;}
 
    inline void setc(node *p, int d) {
        c[d] = p;if(p) p->fa = this;
    }
     
    inline bool d() {
        return fa->c[1] == this;
    }
     
    inline void pushup() {
        data x, y;
        if(c[0]) x = c[0]->v;
        if(c[1]) y = c[1]->v;
        v = merge(x, t, y);
    }
 
    inline void updrev() {
        g.rev ^= true;
        std::swap(v.premin, v.sufmin);
        std::swap(v.premax, v.sufmax);
        v.getans();
    }
 
    inline void updrep(int x) {
        t = x;
        g.rep = x, g.inv = false;
        v.sum = x*v.size;
        if(x == 1) {
            v.premin = v.sufmin = 1;
            v.premax = v.sufmax = v.size;
        }
        else {
            v.premin = v.sufmin = -v.size;
            v.premax = v.sufmax = -1;
        }
        v.getans();
    }
 
    inline void updinv() {
        t = -t;
        g.inv ^= true;
        v.sum = -v.sum;
        data f = v;
        v.premin = -f.premax, v.premax = -f.premin;
        v.sufmin = -f.sufmax, v.sufmax = -f.sufmin;
        v.getans();
    }
     
    inline void pushdown() {
        if(g.rev) {
            for(int i = 0; i < 2; ++i)
                if(c[i]) c[i]->updrev();
            std::swap(c[0], c[1]);
            g.rev = false;
        }
        if(g.rep) {
            for(int i = 0; i < 2; ++i)
                if(c[i]) c[i]->updrep(g.rep);
            g.rep = 0;
        }
        if(g.inv) {
            for(int i = 0; i < 2; ++i)
                if(c[i]) c[i]->updinv();
            g.inv = false;
        }
    }
 
    inline void rot(node *&r) {
        node *f = fa;int p = d();
        if(f == r) r = this, fa = f->fa;
        else f->fa->setc(this, f->d());
        f->setc(c[p^1], p), setc(f, p^1);
        f->pushup();
    }
 
    inline void splay(node *&r) {
        static int n;
        static node *anc[N+9];
        anc[n = 1] = this;
        for(node *p = this; p->fa; p = p->fa)
            anc[++n] = p->fa;
        while(n) anc[n--]->pushdown();
        while(this != r) {
            if(fa != r) {
                if(fa->d() == d()) fa->rot(r);
                else rot(r);
            }
            rot(r);
        }
        for(node *p = this; p; p = p->fa)
            p->pushup();
    }
}*root, pool[N+9], *cur = pool;
 
inline node* newnode(int x) {
    node *ret = cur++;
    ret->t = x, ret->pushup();
    return ret;
}
 
node* maketree(node *&x, int l, int r) {
    if(l == r) {
        x = newnode(s[l]=='('?1:-1);
        return x;
    }
    int mid = (l+r)>>1;
    x = newnode(s[mid]=='('?1:-1);
    if(l < mid) maketree(x->c[0], l, mid-1)->fa = x;
    if(mid < r) maketree(x->c[1], mid+1, r)->fa = x;
    x->pushup();
    return x;
}
 
inline node* find(int pos) {
    node *now = root;
    while(true) {
        now->pushdown();
        int ls = now->c[0]?now->c[0]->v.size:0;
        if(pos <= ls) now = now->c[0];
        else if(pos == ls+1) return now;
        else pos -= ls+1, now = now->c[1];
    }
    return 0;
}
 
inline node* interval(int l, int r) {
    if(l == 1 && r == n) return root;
    else if(l == 1) {
        node *p = find(r+1);
        p->splay(root);
        return p->c[0];
    }
    else if(r == n) {
        node *p = find(l-1);
        p->splay(root);
        return p->c[1];
    }
    else {
        node *p = find(l-1), *q = find(r+1);
        p->splay(root), q->splay(root->c[1]);
        return q->c[0];
    }
}
 
inline void REPLACE(int l, int r, int x) {
    node *p = interval(l, r);
    p->updrep(x), p->splay(root);
}
 
inline void SWAP(int l, int r) {
    node *p = interval(l, r);
    p->updrev(), p->splay(root);
}
 
inline void INVERT(int l, int r) {
    node *p = interval(l, r);
    p->updinv(), p->splay(root);
}
 
inline int QUERY(int l, int r) {
    node *p = interval(l, r);
    return p->v.ans;
}
 
int main() {
    scanf("%d%d%s", &n, &m, s+1);
    maketree(root, 1, n);
    while(m--) {
        int a, b;
        char q[10] = {0};
        scanf("%s%d%d", q, &a, &b);
        if(q[0] == 'R') {
            scanf("%s", q);
            REPLACE(a, b, q[0]=='('?1:-1);
        }
        else if(q[0] == 'S')
            SWAP(a, b);
        else if(q[0] == 'I')
            INVERT(a, b);
        else printf("%d\n", QUERY(a, b));
    }
    return 0;
}
