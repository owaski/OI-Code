#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <climits>
#include <cmath>
#include <ctime>
 
#define debug(...) fprintf(stderr, __VA_ARGS__)
 
const int N = 20;
const int INF = 1e9;
 
int n;
int t[N+9], a[N+9], b[N+9];
int ans = INF;
 
inline bool cmpx(const int &p, const int &q) {
    return b[p] > b[q] || (b[p] == b[q] && a[p] < a[q]);
}
 
inline bool cmpy(const int &p, const int &q) {
    return a[p] > a[q] || (a[p] == a[q] && b[p] < b[q]);
}
 
inline int calc(int *x, int xn, int *y, int yn) {
    int cnta = 0, cntb = 0;
    int dx[N+9] = {0}, dy[N+9] = {0};
    for(int i = 1; i <= xn; ++i) cnta += a[x[i]], dx[i] = cnta;
    for(int i = 1; i <= yn; ++i) cntb += b[y[i]], dy[i] = cntb;
    for(int i = 1; i <= xn; ++i) {
        if(cntb >= dx[i]) cntb += b[x[i]];
        else cntb = dx[i]+b[x[i]];
    }
    for(int i = 1; i <= yn; ++i) {
        if(cnta >= dy[i]) cnta += a[y[i]];
        else cnta = dy[i]+a[y[i]];
    }
    int ret = std::max(cnta, cntb);
    ans = std::min(ans, ret);
    return ret;
}
 
inline void work() {
    int x[N+9] = {0}, xn = 0;
    int y[N+9] = {0}, yn = 0;
    for(int i = 1; i <= n; ++i)
        if(t[i] == 1) x[++xn] = i;
        else y[++yn] = i;
    std::sort(x+1, x+xn+1, cmpx);
    std::sort(y+1, y+yn+1, cmpy);
    int curans = calc(x, xn, y, yn);
    double T = 100, d = 0.99, eps = 1e-5;
    int cnt = 0;
    while(T > eps) {
        T *= d;
        if(xn && rand()%2) {
            int a = rand()%xn+1, b = rand()%xn+1;
            std::swap(x[a], x[b]);
            int tmp = calc(x, xn, y, yn);
            if(tmp < curans) {
                curans = tmp;
                continue;
            }
            else if(rand() < RAND_MAX*exp(-(tmp-curans)/T))
                continue;
            std::swap(x[a], x[b]);
        }
        else if(yn) {            
            int a = rand()%yn+1, b = rand()%yn+1;
            std::swap(y[a], y[b]);
            int tmp = calc(x, xn, y, yn);
            if(tmp < curans) {
                curans = tmp;
                continue;
            }
            else if(rand() < RAND_MAX*exp(-(tmp-curans)/T))
                continue;
            std::swap(y[a], y[b]);
        }
        cnt++;
    }
}
 
void dfs(int now) {
    if(now == n+1) {
        work();
        return ;
    }
    if(t[now] == 3) {
        t[now] = 1;
        dfs(now+1);
        t[now] = 2;
        dfs(now+1);
        t[now] = 3;
    }
    else dfs(now+1);
}
 
int main() {
    scanf("%d", &n);
    for(int i = 1; i <= n; ++i)
        scanf("%d%d%d", t+i, a+i, b+i);
    dfs(1);
    printf("%d\n", ans);
    return 0;
}
