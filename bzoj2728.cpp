#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

#define ll long long

using std::cin;
using std::cout;
using std::endl;

const int K = 60, N = 1000;

int k, n;
ll l, r, a[N+9];

bool vis[K+9];
ll base[K+9];
int basen;

ll calc(ll range) {
    if(range == -1)
        return -1;
    ll ret = 0, max = 0;
    for(int i = 1; i <= basen; ++i)
        if((max|base[i]) <= range)
            max |= base[i], ret |= 1ll<<(basen-i);
    return ret;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    cin >> n >> k >> l >> r;
    for(int i = 1; i <= n; ++i)
        cin >> a[i];
    ll max = (1ll<<k)-1; 
    for(int i = k-1; i >= 0; --i)
        if(!vis[i]) {
            ll val = max;
            for(int j = 1; j <= n; ++j)
                if(a[j]&(1ll<<i)) val &= a[j];
                else val &= max^a[j];
            base[++basen] = val;
            for(int j = 0; j <= i; ++j)
                if(val&(1ll<<j)) vis[j] = true;
        }
    cout << calc(r)-calc(l-1) << endl;
    
    fclose(stdin);fclose(stdout);
    return 0;
}
