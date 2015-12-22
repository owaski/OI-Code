#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <queue>
#define SZ(x) (int)(x).size()
#define debug(...) fprintf(stderr, __VA_ARGS__)

template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}
int OutN;
char Out[20];
template<class T>
inline void write(T x) {
    if(x < 0) putchar('-'), x = -x;
    if(x) {
        OutN = 0;
        while(x) {
            Out[OutN++] = x%10+'0';
            x /= 10;
        }
        while(OutN--)
            putchar(Out[OutN]);
    }
    else putchar('0');
}

const int N = 800009;
const int INF = 1e9;

int n;
int v[2][N];
int cmpd;
struct Line {
    int p[2][2];
    int q[2][2];

    Line() {memset(p, 0, sizeof p);}

    inline void input() {
        for(int i = 0; i < 2; ++i)
            for(int j = 0; j < 2; ++j) {
                read(p[j][i]);
                v[j][++v[j][0]] = p[j][i];
                v[j][++v[j][0]] = p[j][i]+1;
            }
    }

    inline void discrete() {
        for(int i = 0; i < 2; ++i)
            for(int j = 0; j < 2; ++j)
                q[i][j] = std::lower_bound(v[i]+1, v[i]+v[i][0]+1, p[i][j])-v[i];
    }

    inline void swap() {
        for(int i = 0; i < 2; ++i) {
            std::swap(p[i][0], p[i][1]);
            std::swap(q[i][0], q[i][1]);
        }
    }
}l[N];

struct Opt {
    int id, d;

    Opt() {id = d = 0;}
    
    inline void input() {
        read(id), read(d);
    }
}q[N];

int id[N];
int topnum[N];
int topseq[N];

inline double getV(int id, int val) {
    double k = 1.0*(l[id].p[cmpd^1][1]-l[id].p[cmpd^1][0])/(l[id].p[cmpd][1]-l[id].p[cmpd][0]);
    return k*(val-l[id].p[cmpd][0])+l[id].p[cmpd^1][0];
}

struct Data {
    int id;

    Data() {id = 0;}
    Data(int _id) {id = _id;}

    inline friend bool operator < (const Data &p, const Data &q) {
        int tmp = std::max(l[p.id].p[cmpd][0], l[q.id].p[cmpd][0]);
        return getV(p.id, tmp) < getV(q.id, tmp);
    }
};

struct Graph {
    int sz, head[N], to[N], ne[N], ind[N];

    inline void init() {
        sz = 1;
        memset(head, 0, sizeof head);
        memset(ind, 0, sizeof ind);
    }

    inline void addEdge(int u, int v) {
        ind[v]++, to[sz] = v, ne[sz] = head[u], head[u] = sz++;
    }
}G;

std::set<Data> s;
std::vector<int> ins[N], del[N];
std::queue<int> que;

struct SegTree {
    int max[N], min[N], maxf[N], minf[N];

    void build(int x, int l, int r) {
        max[x] = -INF, min[x] = INF;
        maxf[x] = -INF, minf[x] = INF;
        if(l == r) return ;
        int mid = (l+r)>>1;
        build(x<<1, l, mid);
        build(x<<1|1, mid+1, r);
    }

    inline void pushDown(int x) {
        if(maxf[x] != -INF) {
            max[x<<1] = std::max(max[x<<1], maxf[x]);
            max[x<<1|1] = std::max(max[x<<1|1], maxf[x]);
            maxf[x<<1] = std::max(maxf[x<<1], maxf[x]);
            maxf[x<<1|1] = std::max(maxf[x<<1|1], maxf[x]);
            maxf[x] = -INF;
        }
        if(minf[x] != INF) {
            min[x<<1] = std::min(min[x<<1], minf[x]);
            min[x<<1|1] = std::min(min[x<<1|1], minf[x]);
            minf[x<<1] = std::min(minf[x<<1], minf[x]);
            minf[x<<1|1] = std::min(minf[x<<1|1], minf[x]);
            minf[x] = INF;
        }
    }

    inline void pushUp(int x) {
        max[x] = std::max(max[x<<1], max[x<<1|1]);
        min[x] = std::min(min[x<<1], min[x<<1|1]);
    }
    
    void insert(int x, int l, int r, int ll, int rr, int val) {
        if(l == ll && r == rr) {
            max[x] = std::max(max[x], val);
            min[x] = std::min(min[x], val);
            maxf[x] = std::max(maxf[x], val);
            minf[x] = std::min(minf[x], val);
            return ;
        }
        pushDown(x);
        int mid = (l+r)>>1;
        if(rr <= mid) insert(x<<1, l, mid, ll, rr, val);
        else if(ll > mid) insert(x<<1|1, mid+1, r, ll, rr, val);
        else insert(x<<1, l, mid, ll, mid, val), insert(x<<1|1, mid+1, r, mid+1, rr, val);
        pushUp(x);
    }

    std::pair<int,int> get(int x, int l, int r, int ll, int rr) {
        if(l == ll && r == rr) return std::make_pair(max[x], min[x]);
        pushDown(x);int mid = (l+r)>>1;
        if(rr <= mid) return get(x<<1, l, mid, ll, rr);
        else if(ll > mid) return get(x<<1|1, mid+1, r, ll, rr);
        else {
            std::pair<int,int> a = get(x<<1, l, mid, ll, mid);
            std::pair<int,int> b = get(x<<1|1, mid+1, r, mid+1, rr);
            return std::make_pair(std::max(a.first, b.first), std::min(a.second, b.second));
        }
    }
}seg;

inline void init() {
    read(n);
    for(int i = 1; i <= n; ++i)
        l[i].input();
    for(int i = 1; i <= n; ++i)
        q[i].input();
}

inline void discrete() {
    for(int i = 0; i < 2; ++i) {
        std::sort(v[i]+1, v[i]+v[i][0]+1);
        v[i][0] = std::unique(v[i]+1, v[i]+v[i][0]+1)-v[i]-1;
    }
    for(int i = 1; i <= n; ++i)
        l[i].discrete();
}

//0 for vertical, 1 for horizontal

inline bool cmp(const int &a, const int &b) {
    return l[a].p[cmpd][0] < l[b].p[cmpd][0];
}

inline void topSort(int d) {
    G.init();
    for(int i = 1; i <= n; ++i) {
        if(l[i].p[d][0] > l[i].p[d][1])
            l[i].swap();
        //printf("segment[(%d,%d),(%d,%d)]\n", l[i].p[0][0], l[i].p[1][0], l[i].p[0][1], l[i].p[1][1]);
        id[i] = i;
    }
    cmpd = d, s.clear();
    std::sort(id+1, id+n+1, cmp);
    for(int i = 1; i <= v[d][0]; ++i)
        ins[i].clear(), del[i].clear();
    for(int i = 1; i <= n; ++i) {
        ins[l[id[i]].q[d][0]+1].push_back(id[i]);
        del[l[id[i]].q[d][1]].push_back(id[i]);
    }
    for(int i = 1; i <= v[d][0]; ++i) {
        for(int j = 0; j < SZ(ins[i]); ++j) {
            int now = ins[i][j];
            std::set<Data>::iterator a = s.upper_bound(Data(now));
            /*
            if(now == 1) {
                for(std::set<Data>::iterator pp = s.begin(); pp != s.end(); ++pp)
                    debug("%d ", pp->id);
                debug("\n");
            }
            */
            if(a != s.end()) G.addEdge(a->id, now);
            if(a == s.begin()) a = s.end();else --a;
            if(a != s.end()) G.addEdge(now, a->id);
            s.insert(Data(now));
        }
        for(int j = 0; j < SZ(del[i]); ++j) {
            int now = del[i][j];
            std::set<Data>::iterator a = s.find(Data(now));
            ++a;if(a != s.end()) G.addEdge(a->id, now);--a;
            if(a == s.begin()) a = s.end();else --a;
            if(a != s.end()) G.addEdge(now, a->id);
            //debug("%d\n", SZ(s));
            s.erase(Data(now));
            //debug("%d\n", SZ(s));
        }
    }
    int tot = 0;
    for(int i = 1; i <= n; ++i)
        if(G.ind[i] == 0)
            que.push(i);
    //debug("topseq :");
    while(!que.empty()) {
        int now = que.front();que.pop();
        topnum[now] = ++tot, topseq[tot] = now;
        //debug(" %d", now);
        for(int i = G.head[now]; i; i = G.ne[i])
            if((--G.ind[G.to[i]]) == 0)
                que.push(G.to[i]);
    }
    //debug("\n");
}

inline int calc(int d) {
    int ret = n;topSort(d);
    seg.build(1, 1, v[d][0]);
    for(int i = n; i >= 1; --i) {
        std::pair<int,int> cur = seg.get(1, 1, v[d][0],  l[q[i].id].q[d][0]+1, l[q[i].id].q[d][1]);
        if(cur.second == INF && cur.first > -INF) cur.second = cur.first;
        if(cur.first == -INF && cur.second < INF) cur.first = cur.second;
        if(d == 0) {
            if(q[i].d == 1 && cur.second < topnum[q[i].id]) ret = i;
            if(q[i].d == 3 && cur.first > topnum[q[i].id]) ret = i;
        }
        else {
            if(q[i].d == 0 && cur.first > topnum[q[i].id]) ret = i;
            if(q[i].d == 2 && cur.second < topnum[q[i].id]) ret = i;
        }
        seg.insert(1, 1, v[d][0], l[q[i].id].q[d][0]+1, l[q[i].id].q[d][1], topnum[q[i].id]);
    }
    return ret;
}

inline void task1() {
    int ans = std::min(calc(0), calc(1));
    write(ans), putchar('\n');
}

inline void task2() {
    for(int i = 1; i <= n; ++i)
        write(topseq[i]), puts(" 1");
}

inline void solve() {
    discrete();
    task1(), task2();
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
