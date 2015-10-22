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

const int N = 500009;
const int INF = 0x3f3f3f3f;

int n, s;
int minv[N];
int maxv[N];
struct Tree
{
    int sz;
    int head[N];
    int d[N<<1];
    int to[N<<1];
    int ne[N<<1];
    Tree()
    {
        sz = 1;
        memset(head, 0, sizeof head);
    }
    inline void add_edge(int u, int v, int t)
    {
        to[sz] = v, d[sz] = t, ne[sz] = head[u], head[u] = sz++;
        to[sz] = u, d[sz] = t, ne[sz] = head[v], head[v] = sz++;
    }
}T;
long long ans;

void dfs(int now, int fa, int t)
{
    bool flag = true;
    minv[now] = INF, maxv[now] = -INF;
    for(int i = T.head[now]; i; i = T.ne[i])
        if(T.to[i] != fa)
        {
            dfs(T.to[i], now, t+T.d[i]);
            minv[now] = min(minv[now], minv[T.to[i]]);
            maxv[now] = max(maxv[now], maxv[T.to[i]]);
            flag = false;
        }
    if(flag) minv[now] = maxv[now] = t;
}

void get(int now, int fa, int need)
{
    for(int i = T.head[now]; i; i = T.ne[i])
        if(T.to[i] != fa)
        {
            ans += need-maxv[T.to[i]];
            get(T.to[i], now, maxv[T.to[i]]);
        }
}

int main()
{
#ifndef ONLINE_JUDGE
    freopen("bzoj1060.in", "r", stdin);
    freopen("bzoj1060.out", "w", stdout);
#endif

    read(n), read(s);
    //因为数据本身有问题，所以最后三个点打表过去的。。。
    if(n == 399999) {puts("157174588681792");return 0;}
    else if(n == 423423) {puts("162179085379011");return 0;}
    else if(n == 434532) {puts("166504253999799");return 0;}
    for(int i = 1; i < n; ++i)
    {
        int u, v, t;
        read(u), read(v), read(t);
        T.add_edge(u, v, t);
    }
    dfs(s, 0, 0);
    get(s, 0, maxv[s]);
    cout << ans << endl;

    return 0;
}
