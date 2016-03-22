#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

const int N = 4, M = 7, EXT = 8;
const int MOD = 12345678, S = 300;
const int d[][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

int n, m;
char g[N+9][M+9];
int sumX;
int ans;

struct data {
    int x, y;
    data() {x = y = 0;}
    data(int _x, int _y):x(_x), y(_y) {}
}e[EXT+9];
int en;

int cnt[S+9];

int f[N*M+9][S+9];

inline int solve() {
    en = 0;
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= m; ++j)
            if(g[i][j] == 'X')
                e[++en] = data(i, j);
    for(int i = 0; i < (1<<en); ++i) {
        cnt[i] = 0;
        for(int j = 1; j <= en; ++j)
            if(i&(1<<(j-1)))
                g[e[j].x][e[j].y] = '.';
            else g[e[j].x][e[j].y] = 'X';
        for(int p = 1; p <= n; ++p)
            for(int q = 1; q <= m; ++q)
                if(g[p][q] == '.') {
                    bool flag = true;
                    for(int k = 0; k < 8; ++k)
                        if(g[p+d[k][0]][q+d[k][1]] == 'X') {
                            flag = false;
                            break;
                        }
                    if(flag) cnt[i]++;
                }
    }
    for(int i = 1; i <= en; ++i)
        g[e[i].x][e[i].y] = 'X';
    f[0][0] = 1;
    for(int i = 1; i <= n*m; ++i)
        for(int j = 0; j < (1<<en); ++j) {
            int val = 1ll*f[i-1][j]*(cnt[j]-i+1)%MOD;
            for(int k = 1; k <= en; ++k)
                if(j&(1<<(k-1)))
                    (val += f[i-1][j-(1<<(k-1))]) %= MOD;
            f[i][j] = val;
        }
    return f[n*m][(1<<en)-1];
}

void dfs(int x, int y, int cur) {
    if(y == m+1) dfs(x+1, 1, cur);
    else if(x == n+1) {
        //printf("%d\n", ans);
        (ans += ((sumX&1)==(cur&1)?1:-1)*solve()) %= MOD;
        if(ans < 0) ans += MOD;
        return ;
    }
    else {
        if(g[x][y] == 'X')
            dfs(x, y+1, cur+1);
        else {
            dfs(x, y+1, cur);
            for(int k = 0; k < 8; ++k)
                if(g[x+d[k][0]][y+d[k][1]] == 'X')
                    return ;
            g[x][y] = 'X';
            dfs(x, y+1, cur+1);
            g[x][y] = '.';
        }
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; ++i) {
        scanf("%s", g[i]+1);
        for(int j = 1; j <= m; ++j)
            if(g[i][j] == 'X') {
                sumX++;
                for(int k = 0; k < 8; ++k)
                    if(g[i+d[k][0]][j+d[k][1]] == 'X') {
                        puts("0");
                        fclose(stdin);fclose(stdout);
                        return 0;
                    }
            }
    }
    dfs(1, 1, 0);
    printf("%d\n", ans);
    
    fclose(stdin);fclose(stdout);
    return 0;
}
