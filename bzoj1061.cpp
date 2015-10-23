#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 1009;
const int M = 10009;
const int SZ = (2*N+M)<<1;
const int INF = 0x3f3f3f3f;

int n, m;
int need[N];
int S[M], T[M], C[M];
int beg, end;

struct Net
{
    int sz;
    int head[N];
    int to[SZ];
    int f[SZ];
    int c[SZ];
    int ne[SZ];
    Net()
    {
        sz = 1;
        memset(head, 0, sizeof head);
    }
    inline void add_edge(int u, int v, int ff, int cc)
    {
        to[sz] = v, f[sz] = ff, c[sz] = cc, ne[sz] = head[u], head[u] = sz++;
        to[sz] = u, f[sz] = 0, c[sz] = -cc, ne[sz] = head[v], head[v] = sz++;
    }
}G;

int dis[N];
bool vis[N];
int fe[N];
int fv[N];

inline bool spfa()
{
    queue<int> q;
    memset(dis, INF, sizeof dis);
    memset(vis, 0, sizeof vis);
    q.push(beg), vis[beg] = true, dis[beg] = 0;
    while(!q.empty())
    {
        int now = q.front();
        vis[now] = false, q.pop();
        for(int i = G.head[now]; i; i = G.ne[i])
            if(G.f[i] && dis[G.to[i]] > dis[now]+G.c[i])
            {
                fe[G.to[i]] = i;
                fv[G.to[i]] = now;
                dis[G.to[i]] = dis[now]+G.c[i];
                if(!vis[G.to[i]])
                {
                    vis[G.to[i]] = true;
                    q.push(G.to[i]);
                }
            }
    }
    return dis[end] < INF;
}

inline int solve()
{
    int ret = 0, flow = INF;
    for(int i = end; i != beg; i = fv[i])
        flow = min(flow, G.f[fe[i]]);
    for(int i = end; i != beg; i = fv[i])
    {
        ret += flow*G.c[fe[i]];
        G.f[fe[i]] -= flow, G.f[((fe[i]-1)^1)+1] += flow;
    }
    return ret;
}

inline int maxflow_mincost()
{
    int ret = 0;
    while(spfa())
        ret += solve();
    return ret;
}

int main()
{
#ifndef ONLINE_JUDGE
    freopen("bzoj1061.in", "r", stdin);
    freopen("bzoj1061.out", "w", stdout);
#endif

    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; ++i)
        scanf("%d", need+i);
    for(int i = 1; i <= m; ++i)
        scanf("%d%d%d", S+i, T+i, C+i);
    beg = n+2, end = n+3;
    for(int i = 1; i <= n+1; ++i)
    {
        int val = need[i]-need[i-1];
        if(val < 0) G.add_edge(i, end, -val, 0);
        else G.add_edge(beg, i, val, 0);
    }
    for(int i = 1; i <= m; ++i)
        G.add_edge(S[i], T[i]+1, INF, C[i]);
    for(int i = 1; i <= n; ++i)
        G.add_edge(i+1, i, INF, 0);
    int ans = maxflow_mincost();
    printf("%d\n", ans);

    return 0;
}
