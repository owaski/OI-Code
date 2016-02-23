#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

#define ull unsigned long long

ull n;
ull ten[20];
int m;

int a[20], an;

int ans;

inline void mult(int A[3][3], int B[3][3]) {
    int C[3][3] = {0};
    for(int k = 0; k < 3; ++k)
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 3; ++j)
                (C[i][j] += 1ull*A[i][k]*B[k][j]%m) %= m;
    memcpy(A, C, sizeof C);
}

inline void calc(ull l, ull r, ull x) {
    int A[3][3] = {0}, B[3][3] = {0};
    A[0][0] = x%m, A[0][1] = 1;
    A[1][1] = A[1][2] = 1;
    A[2][2] = 1;
    B[0][0] = B[1][1] = B[2][2] = 1;
    ull n = r-l+1;
    while(n) {
        if(n&1) mult(B, A);
        mult(A, A), n >>= 1;
    }
    ans = (1ull*ans*B[0][0]%m+(l%m)*B[0][1]%m+B[0][2])%m;
}

int main() {
    std::cin >> n >> m;
    ten[0] = 1;
    for(int i = 1; i <= 19; ++i)
        ten[i] = ten[i-1]*10;
    ull t = n;
    while(t) a[++an] = t%10, t /= 10;
    for(int i = 1; i < an; ++i)
        calc(ten[i-1], ten[i]-1, ten[i]);
    calc(ten[an-1], n, ten[an]);
    std::cout << ans << std::endl;
    return 0;
}
