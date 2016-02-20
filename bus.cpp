#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
 
#define get(x, y) ((x>>y)&1)
#define mdf(x, y) (x^=(1<<y))
 
const int P = 10;
const int S = 126;
const int MOD = 30031;
 
int n, k, p;
 
int all[S+9];
int alln;
 
int g[S+9][S+9];
int a[S+9][S+9];
 
void getstatus(int now, int s, int last) {
    if(now == k) {
        all[++alln] = s;
        return ;
    }
    for(int i = last+1; i < p; ++i)
        if(!get(s, i-1)) {
            mdf(s, i-1);
            getstatus(now+1, s, i);
            mdf(s, i-1);
        }
}
 
inline int calc(int a, int b) {
    if(get(a, 0)) {
        if(b == ((a>>1)|(1<<(p-1))))
            return 1;
        else return 0;
    }
    int ret = 0;
    for(int i = 1; i <= p; ++i)
        if(get(a, i-1)) {
            int t = a;
            mdf(t, i-1);
            t >>= 1;
            mdf(t, p-1);
            ret += t==b;
        }
    return ret;
}
 
inline void mult(int A[][S+9], int B[][S+9]) {
    int C[S+9][S+9] = {0};
    for(int k = 1; k <= alln; ++k)
        for(int i = 1; i <= alln; ++i)
            for(int j = 1; j <= alln; ++j)
                (C[i][j] += A[i][k]*B[k][j]%MOD) %= MOD;
    memcpy(A, C, sizeof C);
}
 
inline int find() {
    for(int i = 1; i <= alln; ++i) {
        bool flag = true;
        for(int j = 0; j < k; ++j)
            if(!get(all[i], p-j-1)) {
                flag = false;
                break;
            }
        if(flag) return i;
    }
    return -1;
}
 
int main() {
    std::cin >> n >> k >> p;
    getstatus(1, 1<<(p-1), 0);
    for(int i = 1; i <= alln; ++i)
        for(int j = 1; j <= alln; ++j)
            g[j][i] = calc(all[i], all[j]);
    for(int i = 1; i <= alln; ++i)
        a[i][i] = 1;
    n -= k;
    while(n) {
        if(n&1) mult(a, g);
        mult(g, g), n >>= 1;
    }
    int x = find(), ans = a[x][x]; 
    std::cout << ans << std::endl;
    return 0;
}
