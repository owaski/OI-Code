#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 59;
const int INF = N;

int n;
char str[N];
int f[N][N];

inline void init()
{
    scanf("%s", str+1);
    n = strlen(str+1);
}

inline bool same(int x, int y)
{
    if((y-x+1)&1) return false;
    int half = (y-x+1)>>1;
    for(int i = 1; i <= half; ++i)
        if(str[x+i-1] != str[x+i-1+half])
            return false;
    return true;
}

inline void solve()
{
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= n; ++j)
            f[i][j] = j-i+1;   
    for(int l = 1; l <= n; ++l)
        for(int i = 1; i <= n-l+1; ++i)
        {
            int j = i+l-1, cnt = l;
            for(int k = j; k >= i; --k)
                if(same(i, k))
                {
                    cnt -= ((k-i+1)>>1)-1;
                    k -= ((k-i+1)>>1)-1;
                }
            f[i][j] = min(f[i][j], cnt);
            for(int k = i; k < j; ++k)
                f[i][j] = min(f[i][j], f[i][k]+f[k+1][j]+1);
        }
    printf("%d\n", f[1][n]);
}

int main()
{
    freopen("bzoj1068.in", "r", stdin);
    freopen("bzoj1068.out", "w", stdout);

    init();
    solve();

    fclose(stdin);fclose(stdout);
    return 0;
}
