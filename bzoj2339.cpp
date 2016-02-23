#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
 
#define ll long long
 
const int MOD = 1e8+7;
 
int n, m;
int a, b, A, p;
 
inline int powermod(int a, int b) {
    int ret = 1;
    while(b) {
        if(b&1) ret = 1ll*ret*a%MOD;
        a = 1ll*a*a%MOD, b >>= 1;
    }
    return ret;
}
 
int main() {
    int fact = 1;
    std::cin >> n >> m;
    A = a = 1, b = 0, p = powermod(2, n);
    for(int i = 1; i <= m; ++i) {
        int x = A-a-1ll*(i-1)*(p+1-i)%MOD*b%MOD;
        while(x < 0) x += MOD;b = a, a = x;
        A = 1ll*A*(p-i)%MOD, fact = 1ll*fact*i%MOD;
    }
    a = 1ll*a*powermod(fact, MOD-2)%MOD;
    printf("%d\n", a);
    return 0;
}
