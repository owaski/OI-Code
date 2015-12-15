#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int SZ = 1<<16;

int allN;
pair<int,int> all[SZ];

int gcd(int a, int b) {
    return b ? gcd(b, a%b) : a;
}

inline int powerMod(int a, int b, int p) {
    int ret = 1, base = a;
    while(b) {
        if(b&1) ret = 1ll*ret*base%p;
        base = 1ll*base*base%p;
        b >>= 1;
    }
    return ret;
}

int phi(int p) {
    int ret = p;
    for(int i = 2; 1ll*i*i <= p; ++i)
        if(p%i == 0) {
            ret /= i, ret *= i-1;
            while(p%i == 0) p /= i;
        }
    if(p > 1) ret /= p, ret *= p-1;
    return ret;
}

inline int get(int x) {
    int p = lower_bound(all+1, all+allN+1, make_pair(x, 0))-all;
    if(all[p].first == x) return all[p].second;
    else return -1;
}

inline int BSGS(int a, int b, int p) {
    int phip = phi(p);
    int s = ceil(sqrt(1.0*phip));allN = 0;
    for(int i = 0, j = 1; i < s; ++i, j = 1ll*j*a%p)
        all[++allN] = make_pair(j, i);
    sort(all+1, all+allN+1);
    int t = (phip-1)/s, as = powerMod(a, s, p);
    as = powerMod(as, phip-1, p);
    for(int i = 0, j = 1; i <= t; ++i, j = 1ll*j*as%p) {
        int tmp = get(1ll*b*j%p);
        if(tmp != -1) return i*s+tmp;
    }
    return -1;
}

inline void solve(int a, int b, int p, int k) {
    int ans = BSGS(a, b, p);
    if(ans == -1) puts("No Solution");
    else cout << ans+k << endl;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    int a, b, p;
    while(cin >> a >> p >> b) {
    	bool flag;
        if(!a && !b && !p) break;
        a %= p, b %= p; 
        int ta = a, tb = b, tp = p, d, k;
        for(int i = 0; i < 35; ++i)
        	if(powerMod(a, i, p) == b) {
        		cout << i << endl;
        		goto NEXT;
        	}
        flag = true;
        d = gcd(ta, p), k = 0;
        while(d > 1) {
            if(tb%d) {
                flag = false;
                break;
            }
            tb /= d, k++;
            d = gcd(ta, tp /= d);
        }
        if(flag) solve(a%p, 1ll*b*powerMod(powerMod(a, k, p), phi(p)-1, p)%p, p, k);
        else puts("No Solution");NEXT:;
    }
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
