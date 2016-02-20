#include <bits/stdc++.h>
#define clr(a, b) memset(a, b, sizeof a)
#define rep(i, a, b) for(int i = (a); i <= (b); ++i)
#define per(i, a, b) for(int i = (a); i >= (b); --i)
using namespace std;
 
const int MAXN = 200009;
 
struct Dgraph
{
    int sz;
    int head[MAXN];
    int to[MAXN];
    int ne[MAXN];
    Dgraph()
    {
        sz = 1;
        clr(head, 0);
    }
    inline void add_edge(int u, int v)
    {
        to[sz] = v, ne[sz] = head[u], head[u] = sz++;
    }
};
 
struct Node *null;
struct Node
{
    Node *fa, *c[2];
    int sz;bool rev;
    inline void init(int s)
    {
        fa = c[0] = c[1] = null;
        rev = false;sz = s;
    }
    inline void setc(Node *p, int d)
    {
        c[d] = p, p->fa = this;
    }
    inline bool d()
    {
        return fa->c[1] == this;
    }
    inline bool is_root()
    {
        return fa == null || (fa->c[0] != this && fa->c[1] != this);
    }
    inline void upd_rev()
    {
        rev ^= true;
    }
    inline void push_down()
    {
        if(rev)
        {
            rep(i, 0, 1)
                c[i]->upd_rev();
            swap(c[0], c[1]);
            rev = false;
        }
    }
    inline void push_up()
    {
        sz = c[0]->sz+c[1]->sz+1;
    }
    inline void rot()
    {
        Node *f = fa, *ff = f->fa;
        int p = d(), pp = f->d();
        if(f->is_root()) fa = ff;
        else ff->setc(this, pp);
        f->setc(c[p^1], p), setc(f, p^1);
        f->push_up();
    }
    inline void splay()
    {
        int n = 0;
        static Node *anc[MAXN];
        anc[++n] = this;
        for(Node *p = this; !p->is_root(); p = p->fa)
            anc[++n] = p->fa;
        per(i, n, 1)
            anc[i]->push_down();
        while(!is_root())
        {
            Node *f = fa;
            if(!f->is_root())
                if(d() == f->d()) f->rot();
                else rot();
            rot();
        }
        push_up();
    }
    inline void access()
    {
        for(Node *p = this, *q = null; p != null; q = p, p = p->fa)
            p->splay(), p->setc(q, 1), p->push_up();
    }
    inline void be_root()
    {
        access();splay();
        upd_rev();
    }
};
 
int n, m;
int fa[MAXN];
Dgraph G;
Node *cur, pool[MAXN];
Node *node[MAXN];
 
inline void init()
{
    cur = null = pool, cur++, null->init(0);
    rep(i, 1, n+1)
        node[i] = cur++, node[i]->init(1);
}
 
inline void bfs(int s)
{
    queue<int> q;
    q.push(s);
    while(!q.empty())
    {
        int now = q.front();q.pop();
        for(int i = G.head[now]; i; i = G.ne[i])
        {
            int to = G.to[i];
            node[to]->fa = node[now];
            q.push(to);
        }
    }
}
 
inline int query(Node *p)
{
    p->access();p->splay();
    return p->sz-1;
}
 
inline void cut(Node *p, Node *q)
{
    p->be_root();q->access();q->splay();
    p->fa = q->c[0] = null;q->push_up();
}
 
inline void link(Node *p, Node *q)
{
    p->be_root();p->fa = q;
}
 
int main()
{
    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    #endif
     
    scanf("%d", &n);
    rep(i, 1, n)
    {
        int x;
        scanf("%d", &x);
        if(i+x <= n) G.add_edge(fa[i] = i+x, i);
        else G.add_edge(fa[i] = n+1, i);
    }
    init();
    bfs(n+1);
    scanf("%d", &m);
    rep(i, 1, m)
    {
        int x, y, z;
        scanf("%d%d", &x, &y);y++;
        if(x == 1) printf("%d\n", query(node[y]));
        else
        {
            scanf("%d", &z);
            cut(node[y], node[fa[y]]);
            if(y+z <= n) link(node[fa[y] = y+z], node[y]);
            else link(node[fa[y] = n+1], node[y]);
            node[n+1]->be_root();
        }
    }
     
    #ifndef ONLINE_JUDGE
    fclose(stdin);
    fclose(stdout);
    #endif
    return 0;
}
