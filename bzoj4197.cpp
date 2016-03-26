#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

const int N = 500;

int n, MOD;
int prime[N+9], tot;
int sn;
struct data {
    int x, y;
    data() {x = y = 0;}
    data(int _x, int _y):x(_x), y(_y) {}
    inline friend bool operator < (const data &p, const data &q) {
        return p.y < q.y || (p.y == q.y && p.x < q.x);
    }
}a[N+9];
int f[1<<8][1<<8];
int g[2][1<<8][1<<8];

inline bool check(int x) {
    for(int i = 2; i*i <= x; ++i)
        if(x%i == 0)
            return false;
    return true;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%d%d", &n, &MOD);
    for(int i = 2; i <= n; ++i)
        if(check(i))
            prime[++tot] = i;
    for(int i = 1; i <= tot; ++i)
        if(prime[i]*prime[i] > n) {
            sn = i-1;
            break;
        }
    for(int i = 2; i <= n; ++i) {
        int t = i;
        for(int j = 1; j <= sn; ++j)
            while(t%prime[j] == 0) {
                a[i].x |= 1<<(j-1);
                t /= prime[j];
            }
        a[i].y = t;
    }
    std::sort(a+2, a+n+1);
    f[0][0] = 1;
    for(int i = 2; i <= n; ++i) {
        int j = i;
        if(a[i].y > 1)
            while(j < n && a[j+1].y == a[i].y)
                ++j;
        memcpy(g[0], f, sizeof f);
        memcpy(g[1], f, sizeof f);
        for(int k = i; k <= j; ++k) {
            int u = a[k].x;
            for(int x = (1<<sn)-1; x >= 0; --x)
                for(int y = (1<<sn)-1; y >= 0; --y)
                    if(!(x&y)) {
                        if(!(y&u)) (g[0][x|u][y] += g[0][x][y]) %= MOD;
                        if(!(x&u)) (g[1][x][y|u] += g[1][x][y]) %= MOD;
                    }
        }
        for(int x = 0; x < (1<<sn); ++x)
            for(int y = 0; y < (1<<sn); ++y)
                if(!(x&y))
                    (f[x][y] = (g[0][x][y]+g[1][x][y]-f[x][y])%MOD+MOD) %= MOD;
        i = j;
    }
    int ans = 0;
    for(int i = 0; i < (1<<sn); ++i)
        for(int j = 0; j < (1<<sn); ++j)
            if(!(i&j))
                (ans += f[i][j]) %= MOD;
    printf("%d\n", ans);
    
    fclose(stdin);fclose(stdout);
    return 0;
}
