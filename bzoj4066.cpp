#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;
  
template<class T>
inline void read(T &x)
{
    char c;x = 0;
    while((c=getchar()) < '0' || c > '9');
    x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';   
}
  
const int MAXN = 200009;
const int INF = 1e9;
  
struct Pot
{
    int p[2];
    Pot() {}
    Pot(int x, int y) {p[0] = x, p[1] = y;}
    inline void input()
    {
        read(p[0]), read(p[1]);
    }
    inline friend bool operator < (const Pot &a, const Pot &b)
    {
        return a.p[0] != b.p[0] ? a.p[0] < b.p[0] : a.p[1] < b.p[1];
    }
    inline friend bool operator == (const Pot &a, const Pot &b)
    {
        return a.p[0] == b.p[0] && a.p[1] == b.p[1];
    }
    inline friend bool operator <= (const Pot &a, const Pot &b)
    {
        return a.p[0] <= b.p[0] && a.p[1] <= b.p[1];
    }
};
int cmpd;
  
inline bool cmp(const Pot &a, const Pot &b)
{
    return a.p[cmpd] != b.p[cmpd] ? a.p[cmpd] < b.p[cmpd] : a.p[cmpd^1] < b.p[cmpd^1];
}
  
namespace kdt
{
    struct Node *null;
    struct Node
    {
        Pot g, Min, Max;
        int val, sum, sz;
        Node *c[2];
        inline void init(const Pot &a, const int &v)
        {
            g = Min = Max = a;
            val = sum = v, sz = 1;
            c[0] = c[1] = null;
        }
        inline void clear()
        {
            init(g, val);   
        }
        inline void push_up()
        {
            Min = Max = g;
            Min.p[0] = min(Min.p[0], min(c[0]->Min.p[0], c[1]->Min.p[0]));
            Min.p[1] = min(Min.p[1], min(c[0]->Min.p[1], c[1]->Min.p[1]));
            Max.p[0] = max(Max.p[0], max(c[0]->Max.p[0], c[1]->Max.p[0]));
            Max.p[1] = max(Max.p[1], max(c[0]->Max.p[1], c[1]->Max.p[1]));
            sum = val+c[0]->sum+c[1]->sum;
            sz = c[0]->sz+c[1]->sz+1;
        }
    }pool[MAXN], *cur, *root, *st[MAXN];
    int top;
    inline void init()
    {
        root = null = cur = pool;
        null->Min = Pot(INF, INF);
        null->Max = Pot(-INF, -INF);
        null->sum = 0, null->sz = 0;
    }
    inline Node* newnode(const Pot &p, const int &val)
    {
        Node *now = ++cur;
        now->init(p, val);
        return now;
    }
    void get_subtree(Node *now)
    {
        if(now == null) return ;
        st[++top] = now;
        get_subtree(now->c[0]);
        get_subtree(now->c[1]);
    }
    inline bool cmp(const Node *a, const Node *b)
    {
        return cmp(a->g, b->g);   
    }
    Node* make_tree(int l, int r, int d)
    {
        if(l == r)
        {
            st[l]->clear();
            return st[l];
        }
        cmpd = d;
        int mid = (l+r)>>1;
        if(l < mid) nth_element(st+l, st+mid-1, st+r+1, cmp);
        if(mid < r) nth_element(st+mid, st+mid, st+r+1, cmp);
        st[mid]->clear();
        if(l < mid)
            st[mid]->c[0] = make_tree(l, mid-1, d^1);
        if(mid < r)
            st[mid]->c[1] = make_tree(mid+1, r, d^1);
        st[mid]->push_up();
        return st[mid];
    }
    inline Node* rebuild(Node *now)
    {
        top = 0;
        get_subtree(now);
        return make_tree(1, top, 0);
    }
    void add(Node *&now, const Pot &a, int val, int d)
    {
        if(now == null)
        {
            now = newnode(a, val);
            return ;
        }
        if(now->g == a)
        {
            now->val += val;
            now->sum += val;
            return ;
        }
        cmpd = d;
        if(cmp(a, now->g)) add(now->c[0], a, val, d^1);
        else add(now->c[1], a, val, d^1);now->push_up();
        int max_s = 0;
        max_s = max(now->c[0]->sz, now->c[1]->sz);
        if(max_s >= now->sz*0.9)
            now = rebuild(now);
    }
    inline bool inside(const Pot &a, const Pot &b, const Pot &c)
    {
        return b <= a && a <= c;
    }
    inline bool cross(const Pot &a, const Pot &b, const Pot &c, const Pot &d)
    {
        Pot x, y;
        x = Pot(max(a.p[0], c.p[0]), max(a.p[1], c.p[1]));
        y = Pot(min(b.p[0], d.p[0]), min(b.p[1], d.p[1]));
        return x.p[0] <= y.p[0] && x.p[1] <= y.p[1];
    }
    int qry(Node *now, const Pot &a, const Pot &b, int d)
    {
        if(now == null) return 0;
        if(a <= now->Min && now->Max <= b)
            return now->sum;
        int ret = 0;cmpd = d;
        if(a <= now->g && now->g <= b)
            ret += now->val;
        if(cross(now->c[0]->Min, now->c[0]->Max, a, b))
            ret += qry(now->c[0], a, b, d^1);
        if(cross(now->c[1]->Min, now->c[1]->Max, a, b))
            ret += qry(now->c[1], a, b, d^1);
        return ret;
    }
}
  
int n;
int last_ans;
  
int main()
{
#ifndef ONLINE_JUDGE
    freopen("bzoj4066.in", "r", stdin);
    freopen("bzoj4066.out", "w", stdout);
#endif
  
    read(n);
    kdt::init();
    while(true)
    {
        int type;
        read(type);
        if(type == 1)
        {
            Pot p;int val;
            p.input(), read(val);
            p.p[0] ^= last_ans, p.p[1] ^= last_ans, val ^= last_ans;
            kdt::add(kdt::root, p, val, 0);
        }
        else if(type == 2)
        {
            Pot p, q;
            p.input(), q.input();
            p.p[0] ^= last_ans, p.p[1] ^= last_ans;
            q.p[0] ^= last_ans, q.p[1] ^= last_ans;
            last_ans = kdt::qry(kdt::root, p, q, 0);
            printf("%d\n", last_ans);
        }
        else break;
    }
  
    return 0;   
}
