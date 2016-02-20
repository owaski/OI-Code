#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
 
const int N = 200, M = N, P = 10;
 
int n, m, p;
int sum[N+9][M+9];
 
int C[N+9][M+9];
int A[N+9][M+9];
 
void dfs(int now, int l[], int r[]) {
    if(now == m) {
        for(int i = 1; i < n; ++i)
            A[i][0] = l[i];
        for(int i = 1; i < n; ++i)
            for(int j = 1; j < m; ++j)
                A[i][j] = sum[i][j]-A[i-1][j]-A[i][j-1]-A[i-1][j-1];
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < m; ++j)
                printf("%d%c", A[i][j], j==m-1?'\n':' ');
        exit(0);
    }
    int *tl = new int[N+9], *tr = new int[N+9];
    for(int i = 0; i < p; ++i) {
        A[0][now] = i;
        memcpy(tl, l, (M+9)*sizeof(int));
        memcpy(tr, r, (M+9)*sizeof(int));
        bool flag = true;
        for(int j = 1; j < n; ++j) {
            int x = j&1?-1:1, y = now&1?-1:1;
            int val = C[j][now]+A[0][0]*((now+j)&1?1:-1)+x*i;
            if((val < 0 && y == -1) || (val >= p && y == 1)) {
                flag = false;
                break;
            }
            if(val < 0) tl[j] = std::max(tl[j], -val);
            else if(val >= p) tl[j] = std::max(tl[j], val-p+1);
            else {
                if(y == 1) tr[j] = std::min(tr[j], p-val-1);
                else tr[j] = std::min(tr[j], val);
            }
            if(tl[j] > tr[j]) {
                flag = false;
                break;
            }
        }
        if(flag) dfs(now+1, tl, tr);
    }
}
 
int main() {
    scanf("%d%d%d", &n, &m, &p);
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < m; ++j)
            scanf("%d", &sum[i][j]);
    for(int i = 1; i < n; ++i)
        for(int j = 1; j < m; ++j)
            C[i][j] = sum[i][j]-C[i-1][j]-C[i][j-1]-C[i-1][j-1];
    int *l = new int[N+9], *r = new int[N+9];
    for(int i = 1; i < n; ++i)
        l[i] = 0, r[i] = p-1;
    for(int i = 0; i < p; ++i)
        A[0][0] = i, dfs(1, l, r);
    return 0;
}
