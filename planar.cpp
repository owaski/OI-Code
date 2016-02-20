#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
 
template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-') {
        c++, c--;
    }
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}
 
const int N = 200, M = 3*N, MM = 10000;
 
int n, m;
struct edge {
    int u, v;
    edge() {u = v = 0;}
    edge(int _u, int _v):u(_u), v(_v) {}
    inline void input() {
        read(u), read(v);
    }
}e[MM+9];
int path[N+9], flag[N+9];
 
struct graph {
    int sz, head[M+9], to[M*M+9], ne[M*M+9];
    inline void init() {
        sz = 1, memset(head, 0, sizeof head);
    }
    inline void addedge(int u, int v) {
        to[sz] = v, ne[sz] = head[u], head[u] = sz++;
        to[sz] = u, ne[sz] = head[v], head[v] = sz++;
    }
}g;
 
int col[M+9];
 
bool dfs(int now, int c) {
    col[now] = c, c = 3-c;
    for(int i = g.head[now], to; i; i = g.ne[i])
        if(col[to = g.to[i]]) {
            if(col[to] != c) return false;
        }
        else if(!dfs(to, c))
            return false;
    return true;
}
 
int main() {
    int Case;
    read(Case);
    while(Case--) {
        read(n), read(m);
        for(int i = 1; i <= m; ++i)
            e[i].input();
        for(int i = 1; i <= n; ++i)
            read(path[i]);
        if(m > 3*n-6) {
            puts("NO");
            continue;
        }
        for(int i = 1; i <= n; ++i)
            flag[path[i]] = i;
        g.init();
        for(int i = 1; i <= m; ++i) {
            int a = flag[e[i].u], b = flag[e[i].v];
            if(a > b) std::swap(a, b);
            for(int j = i+1; j <= m; ++j) {
                int x = flag[e[j].u], y = flag[e[j].v];
                if(x > y) std::swap(x, y);
                if(a < x && x < b && b < y || x < a && a < y  && y < b)
                    g.addedge(i, j);
            }
        }
        memset(col, 0, sizeof col);
        bool ans = true;
        for(int i = 1; i <= m; ++i)
            if(!col[i] && !dfs(i, 1)) {
                ans = false;
                break;
            }
        if(ans) puts("YES");
        else puts("NO");
    }
    return 0;
}
﻿
