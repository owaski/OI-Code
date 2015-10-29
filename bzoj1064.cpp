#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
 
template<class T>
inline void read(T &x)
{
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}
 
const int N = 100009;
const int M = 1000009;
 
int n, m;
struct Graph
{
    int sz;
    int head[N];
    int flag[M<<1];
    int to[M<<1];
    int ne[M<<1];
    Graph()
    {
        sz = 1;
        memset(head, 0, sizeof head);
    }
    inline void add_edge(int u, int v, int a)
    {
        to[sz] = v, flag[sz] = a, ne[sz] = head[u], head[u] = sz++;
    }
}G;
 
bool vis[N];
int dis[N];
int ans1, ans2;
 
inline int gcd(int a, int b)
{
    return b?gcd(b, a%b):a;
}
 
inline void init()
{
    read(n), read(m);
    for(int i = 1; i <= m; ++i)
    {
        int u, v;
        read(u), read(v);
        G.add_edge(u, v, 1);
        G.add_edge(v, u, -1);
    }
}
 
void dfs(int now)
{
    vis[now] = true;
    for(int i = G.head[now]; i; i = G.ne[i])
        if(!vis[G.to[i]])
        {
            dis[G.to[i]] = dis[now]+G.flag[i];
            dfs(G.to[i]);
        }
        else if(dis[now]-dis[G.to[i]]+G.flag[i] > 0)
			ans2 = gcd(ans2, dis[now]-dis[G.to[i]]+G.flag[i]);
}
 
inline int bfs(int s)
{
    int l = 0, r = 0;
    static pair<int,int> q[N];
    q[++r] = make_pair(s, 0), vis[s] = true;
    int minr = 0, maxr = 0;
    while(l < r)
    {
        pair<int,int> cur = q[++l];
        int now = cur.first, d = cur.second;
        minr = min(minr, d), maxr = max(maxr, d);
        for(int i = G.head[now]; i; i = G.ne[i])
            if(!vis[G.to[i]])
            {
                vis[G.to[i]] = true;
                q[++r] = make_pair(G.to[i], d+G.flag[i]);   
            }
    }
    return maxr-minr+1;
}
 
inline void solve()
{
    for(int i = 1; i <= n; ++i)
        if(!vis[i])
            dfs(i);
    if(ans2)
    {
        if(ans2 < 3) 
            ans1 = ans2 = -1;
        else
        {
            ans1 = 3;
            while(ans2%ans1) ans1++;
        }
        return ;
    }
    memset(vis, 0, sizeof vis);
    for(int i = 1; i <= n; ++i)
        if(!vis[i])
            ans2 += bfs(i);
    if(ans2 < 3) ans1 = ans2 = -1;
    else ans1 = 3;
}
 
inline void output()
{
    printf("%d %d\n", ans2, ans1);
}
 
int main()
{
	freopen("bzoj1064.in", "r", stdin);
	freopen("bzoj1064.out", "w", stdout);
	
    init();
    solve();
    output();
	
	fclose(stdin);fclose(stdout);
    return 0;
}
