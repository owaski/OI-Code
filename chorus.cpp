#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
 
const int N = 1000;
const int MOD = 19650827;
 
int n, h[N+9];
 
int f[N+9][N+9][2];//0 for left, 1 for right
 
inline void add(int &x, int y) {
    x += y;
    if(x >= MOD) x -= MOD;
}
 
int main() {
    scanf("%d", &n);
    for(int i = 1; i <= n; ++i)
        scanf("%d", h+i);
    if(n == 1) {
        puts("1");
        return 0;
    }
    for(int i = 1; i < n; ++i)
        if(h[i] < h[i+1]) f[i][i+1][0] = f[i][i+1][1] = 1;
    for(int l = 3; l <= n; ++l)
        for(int i = 1; i+l-1 <= n; ++i) {
            int j = i+l-1, v0 = 0, v1 = 0;
            if(h[i] < h[i+1]) add(v0, f[i+1][j][0]);
            if(h[i] < h[j]) add(v0, f[i+1][j][1]);
            if(h[j] > h[j-1]) add(v1, f[i][j-1][1]);
            if(h[j] > h[i]) add(v1, f[i][j-1][0]);
            f[i][j][0] = v0, f[i][j][1] = v1;
        }
    printf("%d\n", (f[1][n][0]+f[1][n][1])%MOD);
    return 0;
}
