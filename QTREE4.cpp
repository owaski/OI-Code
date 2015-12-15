#include <set>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define SZ(x) (int)(x).size()
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

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

const int N = 100009;
const int INF = 1e9+7;

int n;
struct Tree {
    int sz;
    int head[N];
    int to[N<<1];
    int c[N<<1];
    int ne[N<<1];
    Tree() {
        sz = 1;
        memset(head, 0, sizeof head);
    }
    inline void addEdge(int u, int v, int w) {
        to[sz] = v, c[sz] = w, ne[sz] = head[u], head[u] = sz++;
        to[sz] = u, c[sz] = w, ne[sz] = head[v], head[v] = sz++;
    }
}G;
int fa[N];
int dis[N];
int sz[N];
int pref[N];
int pos[N];
int curp;
int pathR[N];
int root[N];
int csz[N];
pair<int,int> d[N];
vector<int> h[N];
vector<int> hId[N];
int hPos[N];
multiset<int> ans;
bool col[N];
struct Node {
    int lc, rc;
    int lp, rp;
    int maxL, maxR, opt;
    Node() {lc = rc = lp = rp = maxL = maxR = opt = 0;}
}tree[N<<1];
int total;

inline void init() {
    read(n);
    for(int i = 1; i < n; ++i) {
        int a, b, c;
        read(a), read(b), read(c);
        G.addEdge(a, b, c);
    }
}

inline pair<int,int> getD(int u) {
    int sz = SZ(h[u])-1;
    if(sz == 0) return make_pair(-INF, -INF);
    else if(sz == 1) return make_pair(h[u][1], -INF);
    else if(sz == 2) return make_pair(h[u][1], h[u][2]);
    else return make_pair(h[u][1], max(h[u][2], h[u][3]));
}

inline void swap(int p, int x, int y) {
    swap(h[p][x], h[p][y]);
    swap(hId[p][x], hId[p][y]);
    swap(hPos[hId[p][x]], hPos[hId[p][y]]);
}

void shiftUp(int p, int x) {
    int y = x>>1;
    if(y && h[p][x] >= h[p][y]) {
        swap(p, x, y);
        shiftUp(p, y);
    }
}

void shiftDown(int p, int x) {
    int y = x<<1, z = y|1;
    if(y < SZ(h[p]) && h[p][y] >= h[p][x] && (z >= SZ(h[p]) || h[p][y] >= h[p][z])) {
        swap(p, x, y);
        shiftDown(p, y);
    }
    if(z < SZ(h[p]) && h[p][z] >= h[p][y] && h[p][z] >= h[p][x]) {
        swap(p, x, z);
        shiftDown(p, z);
    }
}

inline void insert(int p, int id, int val) {
    h[p].push_back(val), hPos[id] = SZ(h[p])-1, hId[p].push_back(id);
    shiftUp(p, SZ(h[p])-1);
}

inline int dist(int x, int y) {
    return dis[y]-dis[x];
}

inline void update(int x) {
    Node &p = tree[x], a = tree[tree[x].lc], b = tree[tree[x].rc];
    p.maxL = max(a.maxL, dist(a.lp, b.lp)+b.maxL);
    p.maxR = max(b.maxR, dist(a.rp, b.rp)+a.maxR);
    p.opt = max(max(a.opt, b.opt), a.maxR+b.maxL+dist(a.rp, b.lp));
}

void makeTree(int cur, int l, int r, int *path) {
    tree[cur].lp = path[l], tree[cur].rp = path[r];
    if(l == r) {
        tree[cur].maxL = tree[cur].maxR = d[path[l]].first;
        tree[cur].opt = d[path[l]].first+max(0, d[path[l]].second);
        return ;
    }
    int mid = (l+r)>>1;
    makeTree(tree[cur].lc = ++total, l, mid, path);
    makeTree(tree[cur].rc = ++total, mid+1, r, path);
    update(cur);
}

inline void pre() {
    int h = 0, t = 0;
    static int q[N];
    static int path[N];
    static bool vis[N];
    q[++t] = 1, vis[1] = true;
    while(h < t) {
        int now = q[++h], to;
        for(int i = G.head[now]; i; i = G.ne[i])
            if(!vis[to = G.to[i]]) {
                fa[to] = now;
                dis[to] = dis[now]+G.c[i];
                vis[to] = true;
                q[++t] = to;
            }
    }
    for(int i = n; i >= 2; --i)
        sz[fa[q[i]]] += (++sz[q[i]]);
    for(int i = 1; i <= n; ++i)
        for(int j = G.head[i]; j; j = G.ne[j])
            if(G.to[j] != fa[i] && sz[pref[i]] < sz[G.to[j]])
                pref[i] = G.to[j];
    for(int i = 1; i <= n; ++i) {
        int cur = q[i];
        if(!pathR[cur]) pathR[cur] = cur;
        if(pref[cur]) pathR[pref[cur]] = pathR[cur];
    }
    /*
    for(int i = 1; i <= n; ++i)
        debug("%d : fa = %d, dis = %d, sz = %d, pref = %d, pathR = %d\n", i, fa[i], dis[i], sz[i], pref[i], pathR[i]);
    */
    for(int i = n; i >= 1; --i) {
        int cur = q[i];
        ::h[cur].push_back(0);
        hId[cur].push_back(0);
        for(int j = G.head[cur]; j; j = G.ne[j])
            if(G.to[j] != fa[cur] && G.to[j] != pref[cur])
                insert(cur, G.to[j], tree[root[G.to[j]]].maxL+G.c[j]);
        if(cur != pathR[cur]) continue;
        for(int j = cur; j; j = pref[j]) {
            pos[j] = ++curp;
            d[j] = getD(j);
            d[j] = make_pair(max(0, d[j].first), max(0, d[j].second));
            path[++csz[cur]] = j;
        }
        makeTree(root[cur] = ++total, 1, csz[cur], path);
        ans.insert(-tree[root[cur]].opt);
    }
}

void modify(int cur, int l, int r, int u) {
    if(l == r) {
        tree[cur].maxL = tree[cur].maxR = d[u].first;
        if(!col[u]) tree[cur].opt = d[u].first+max(0, d[u].second);
        else tree[cur].opt = d[u].first+d[u].second;
        return ;
    }
    int mid = (l+r)>>1;
    if(pos[u] <= mid) modify(tree[cur].lc, l, mid, u);
    else modify(tree[cur].rc, mid+1, r, u);
    update(cur);
}

inline void adjust(int u) {
    if(u == 1) return ;
    int pos = hPos[u];
    h[fa[u]][pos] = tree[root[u]].maxL+dis[u]-dis[fa[u]];
    shiftUp(fa[u], pos), shiftDown(fa[u], pos);
}

int sum;

inline void modify(int u) {
    col[u] ^= true;
    sum += col[u] ? -1 : 1;
    while(u) {
        d[u] = getD(u);
        if(!col[u]) d[u] = make_pair(max(0, d[u].first), max(0, d[u].second));
        int val = tree[root[pathR[u]]].opt;ans.erase(ans.lower_bound(-val));
        modify(root[pathR[u]], pos[pathR[u]], pos[pathR[u]]+csz[pathR[u]]-1, u);
        adjust(pathR[u]), ans.insert(-tree[root[pathR[u]]].opt), u = fa[pathR[u]];
    }
}

inline void solve() {
    pre();
    sum = n;
    int q;read(q);
    while(q--) {
        char t[3];
        scanf("%s", t);
        if(t[0] == 'C') {
            int x;
            read(x);
            modify(x);
        }
        else {
            if(sum)
                write(-*ans.begin()), putchar('\n');
            else puts("They have disappeared.");
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
