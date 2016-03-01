#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>

#define debug(...) fprintf(stderr, __VA_ARGS__)

#define vector std::vector
#define pb push_back
#define SZ(x) (int)(x).size()

const int N = 100000;
const int LOG = 17;
const int MOD = 1000000001;

int n;
bool ok[(1<<LOG)+9];
bool vis[N+9];
vector<int> g[LOG+9];
int f[LOG+9][(1<<LOG)+9];
int ans = 1;

inline bool check(int x) {
    int last = x&1;
    for(int i = 1; i < LOG; ++i) {
        if(last&((x>>i)&1))
            return false;
        last = (x>>i)&1;
    }
    return true;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%d", &n);
    for(int i = 0; i < (1<<LOG); ++i)
        if(check(i)) ok[i] = true;
    int cur = 0;
    while(cur < n) {
        if(vis[++cur]) continue;
        for(int i = 0; i < LOG; ++i)
            g[i].clear();
        for(int i = 0; i < LOG; ++i) {
            int p = i==0?cur:g[i-1][0]*2;
            if(p > n) break;
            while(p <= n) {
                g[i].pb(p);
                vis[p] = true;
                p *= 3;
            }
        }
        f[0][0] = 1;
        for(int i = 0; i <= LOG; ++i)
            if(g[i].empty()) {
                int sum = 0;
                for(int j = 0; j < (1<<SZ(g[i-1])); ++j)
                    if(ok[j] && f[i][j]) {
                        (sum += f[i][j]) %= MOD;
                        f[i][j] = 0;
                    }
                ans = 1ll*ans*sum%MOD;
                break;
            }
            else {
                int last = i==0?0:SZ(g[i-1]);
                for(int j = 0; j < (1<<last); ++j)
                    if(ok[j] && f[i][j]) {
                        int v = f[i][j];
                        for(int k = 0; k < (1<<SZ(g[i])); ++k)
                            if(ok[k] && !(j&k))
                                (f[i+1][k] += v) %= MOD;
                        f[i][j] = 0;
                    }
            }
    }
    printf("%d\n", ans);
    
    fclose(stdin);fclose(stdout);
    return 0;
}
