#include <bits/stdc++.h>
using namespace std;

const int MAXN = 109;

int n, T;
int a[MAXN];
int id[MAXN];
int b[MAXN], tot;
int g[MAXN][MAXN];
int p[MAXN][MAXN];
int d[MAXN][MAXN];
int f[MAXN][MAXN];
int maxf[MAXN];
int ans;

inline void spfa(int s)
{
    queue<int> q;
    bool vis[MAXN];
    memset(vis, 0, sizeof vis);
    p[s][s] = 1, vis[s] = 1, q.push(s);
    while(!q.empty())
    {
        int now = q.front();
        q.pop(), vis[now] = 0;
        for(int i = 1; i <= n; ++i)
            if(g[now][i])
                if(p[s][i] < p[s][now]+1)
                {
                    p[s][i] = p[s][now]+1;
                    if(!vis[i])
                    {   
                        vis[i] = 1;
                        q.push(i);
                    }
                }
    }
}

int main()
{
#ifndef ONLINE_JUDGE
    freopen("D.in", "r", stdin);
    freopen("D.out", "w", stdout);
#endif

    scanf("%d%d", &n, &T);
    for(int i = 1; i <= n; ++i)
    {
        scanf("%d", a+i);
        b[++tot] = a[i];
    }
    sort(b+1, b+tot+1);
    tot = unique(b+1, b+tot+1)-b-1;
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= tot; ++j)
            if(a[i] == b[j])
                id[i] = j;
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j < i; ++j)
            if(a[j] <= a[i])
                g[j][i] = 1;
    for(int i = 1; i <= n; ++i)
        spfa(i);
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= n; ++j)
            d[id[i]][id[j]] = max(d[id[i]][id[j]], p[i][j]);
    for(int l = 1; l <= tot; ++l)
        for(int i = 1; i+l-1 <= tot; ++i)
            d[i][i+l-1] = max(d[i][i+l-1], max(d[i+1][i+l-1], d[i][i+l-2]));
    for(int i = 1; i <= tot; ++i)
        maxf[i] = max(maxf[i-1], d[i][i]);
    for(int k = 1; k <= min(tot, T); ++k)
        for(int i = 1; i <= tot; ++i)
        {
            for(int j = 0; j <= i; ++j)
                f[i][k] = max(f[i][k], f[j][k-1]+d[j][i]);
            ans = max(ans, f[i][k]+(T-k)*maxf[i]);
        }
    printf("%d\n", ans);

    return 0;   
}
