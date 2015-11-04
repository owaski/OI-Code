#include <bits/stdc++.h>
#define V(a, b) ((a)*n+b)
#define INV(x) ((((x)-1)^1)+1)
using namespace std;

const int N = 62;
const int M = 12;
const int S = N*M+N;
const int E = (S*N+N)<<1;
const int INF = 1e9;

int n, m;
struct Net
{
    int sz;
    int head[S];
    int to[E];
    int f[E];
    int c[E];
    int ne[E];
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
int s, t;
int fv[S];
int fe[S];
int dis[S];
bool vis[S];

inline void init()
{
    scanf("%d%d", &m, &n);
    s = n+n*m+1, t = n+n*m+2;
    for(int i = 1; i <= n; ++i)
        G.add_edge(s, i, 1, 0);
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= m; ++j)
        {
            int tmp;scanf("%d", &tmp);
            for(int k = 1; k <= n; ++k)
                G.add_edge(i, V(j, k), 1, k*tmp);
        }
    for(int i = n+1; i <= n+n*m; ++i)
        G.add_edge(i, t, 1, 0);
}

inline bool spfa()
{
    queue<int> q;
    for(int i = 1; i <= n+n*m+2; ++i)
    { 
        vis[i] = false, dis[i] = INF;
        fv[i] = fe[i] = 0;
    }
    q.push(s), vis[s] = true, dis[s] = 0;
    while(!q.empty())
    {
        int now = q.front();
        q.pop();vis[now] = false;
        for(int i = G.head[now]; i; i = G.ne[i])
            if(G.f[i] && dis[G.to[i]] > dis[now]+G.c[i])
            {
                fv[G.to[i]] = now;
                fe[G.to[i]] = i;
                dis[G.to[i]] = dis[now]+G.c[i];
                if(!vis[G.to[i]])
                {
                    q.push(G.to[i]);
                    vis[G.to[i]] = true;
                }
            }
    }
    return dis[t] < INF;
}

inline int get()
{
    int flow = INF, ret = 0;
    for(int i = t; i != s; i = fv[i])
        flow = min(flow, G.f[fe[i]]);
    for(int i = t; i != s; i = fv[i])
    {
        G.f[fe[i]] -= flow, G.f[INV(fe[i])] += flow;
        ret += G.c[fe[i]]*flow;
    }
    return ret;
}

inline int maxFlowMinCost()
{
    int ret = 0;
    while(spfa())
        ret += get();
    return ret;
}

inline void solve()
{
    printf("%.2lf\n", (double)maxFlowMinCost()/n);
}

int main()
{
    init();
    solve();
    return 0;
}
