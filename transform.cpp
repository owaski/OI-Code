#include <bits/stdc++.h>
#define INV(x) ((((x)-1)^1)+1)
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 40009;

int n;
struct Graph
{
    int sz;
    int head[N];
    int to[N<<1];
    int ne[N<<1];
    int fr[N<<1];
    int deg[N];
    Graph()
    {
        sz = 1;
        memset(head, 0, sizeof head);
    }
    inline void addEdge(int u, int v)
    {
        deg[u]++, to[sz] = v, fr[head[u]] = sz, ne[sz] = head[u], head[u] = sz++;
        deg[v]++, to[sz] = u, fr[head[v]] = sz, ne[sz] = head[v], head[v] = sz++;
    }
    inline void delEdge(int id)
    {
        int inv = INV(id);
        if(id == head[to[inv]]) head[to[inv]] = ne[id];
        if(inv == head[to[id]]) head[to[id]] = ne[inv];
        deg[to[id]]--, deg[to[inv]]--;
        ne[fr[id]] = ne[id], fr[ne[id]] = fr[id];
        ne[fr[inv]] = ne[inv], fr[ne[inv]] = fr[inv];
    }
}G;

int Stack[N];
int Stop;

bool del[N];

int mat[N];

int move(int x, int y)
{
    x--;
    x += y, x %= n;
    if(x < 0) x += n;
    return ++x;
}

void init()
{
    scanf("%d", &n);
    for(int i = 1; i <= n; ++i)
    {
        int dis, a, b;
        scanf("%d", &dis);
        a = move(i, dis);
        b = move(i, -dis);
        if(a > b) swap(a, b);
        G.addEdge(i, b+n);
        G.addEdge(i, a+n);
    }
}

void noAnswer()
{
    puts("No Answer");
    exit(0);
}

void Match(int x, int y)
{
    mat[x] = y, mat[y] = x;
}

void cut()
{
    for(int i = 1; i <= n; ++i)
        if(G.deg[i+n] == 0)
            noAnswer();
        else if(G.deg[i+n] == 1)
            Stack[++Stop] = i+n;
    while(Stop)
    {
        int p = Stack[Stop--];
        del[p] = true;
        int edge = G.head[p];
        Match(p, G.to[edge]), p = G.to[edge];
        G.delEdge(edge), del[p] = true;
        for(int i = G.head[p]; i; i = G.ne[i])
        {
            p = G.to[i];G.delEdge(i);
            if(G.deg[p] == 0)
                noAnswer();
            else if(G.deg[p] == 1)
                Stack[++Stop] = p;
        }
    }
}

void dfs(int now, int f)
{
    del[now] = true;
    for(int i = G.head[now]; i; i = G.ne[i])
        if(!del[G.to[i]])
        {
            dfs(G.to[i], f^1);
            if(!f) Match(now, G.to[i]);
        }
}

void solve()
{
    cut();
    for(int i = 1; i <= n; ++i)
        if(!del[i])
            dfs(i, 0);
    for(int i = 1; i <= n; ++i)
        printf("%d%c", mat[i]-n-1, i==n?'\n':' ');
}

int main()
{
    freopen("transform.in", "r", stdin);
    freopen("transform.out", "w", stdout);

    init();
    solve();

    fclose(stdin);fclose(stdout);
    return 0;
}
