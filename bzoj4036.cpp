#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

const int N = 20, M = 1<<N;
const double eps = 1e-8;

int n, m;
double p[M+9];
double f[M+9];

inline void transform(double *A, int f) {
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < m; ++j)
            if(j&(1<<i))
                A[j] += f*A[j^(1<<i)];
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%d", &n), m = 1<<n;
    for(int i = 0; i < m; ++i)
        scanf("%lf", p+i);
    transform(p, 1);
    for(int i = 0; i < m; ++i)
        if(p[i] > 1-eps) f[i] = 0;
        else f[i] = 1/(p[i]-1);
    transform(f, -1);
    if(f[m-1] > eps) printf("%.10lf\n", f[m-1]);
    else puts("INF");
    
    fclose(stdin);fclose(stdout);
    return 0;
}
