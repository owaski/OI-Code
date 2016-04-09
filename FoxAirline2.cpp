#include <bits/stdc++.h>

#define debug(...) fprintf(stderr, __VA_ARGS__)

using namespace std;

const int N = 10;

int bel[N+9];
bool ans = true;

void dfs(int now, int par, int n, vector<int> &a, vector<int> &b) {
    if(now == n) {
        int c = 0;
        for(int i = 0; i < a.size(); ++i)
            if(bel[a[i]] != bel[b[i]])
                c++;
        if(c < 2*par) ans = false;
        return ;
    }
    if(!ans) return ;
    for(int i = 0; i <= par; ++i) {
        bel[now] = i;
        dfs(now+1, par, n, a, b);
    }
    bel[now] = par+1;
    dfs(now+1, par+1, n, a, b);
    bel[now] = -1;
}

struct FoxAirline2 {
    inline string isPossible(int n, vector<int> a, vector<int> b) {
        memset(bel, -1, sizeof bel);
        dfs(0, -1, n, a, b);
        return ans ? "Possible" : "Impossible";
    }
};
