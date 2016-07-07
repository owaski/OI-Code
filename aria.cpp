#include <bits/stdc++.h>

#define ll long long

#define pii std::pair<int,int>
#define mp std::make_pair
#define SZ(x) (int)(x).size()
#define pb push_back

template<class T>inline void chkmax(T &x, const T &y) {if(x < y) x = y;}
template<class T>inline void chkmin(T &x, const T &y) {if(x > y) x = y;}

template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}
static int outn;
static char out[(int)2e7];
template<class T>
inline void write(T x) {
    if(x < 0) out[outn++] = '-', x = -x;
    if(x) {
        static int tmpn;
        static char tmp[20];
        tmpn = 0;
        while(x) tmp[tmpn++] = x%10+'0', x /= 10;
        while(tmpn) out[outn++] = tmp[--tmpn];
    }
    else out[outn++] = '0';
}

const int N = 40;
const int S = 1 << 20;

int n, a[N + 9];
ll tot, x[S + 9], y[S + 9];

int get(int l, int r, ll *v) {
    int vn = 0;
    v[++vn] = 0;
    static ll g[S + 9], h[S + 9];
    for(int i = l; i <= r; ++i) {
        for(int j = 1; j <= vn; ++j) g[j] = v[j] + a[i];
        for(int j = 1, k = 1, t = 0; j <= vn || k <= vn; )
            if(j <= vn && (k > vn || v[j] <= g[k])) h[++t] = v[j++];
            else h[++t] = g[k++];
        vn <<= 1;
        std::copy(h + 1, h + vn + 1, v + 1);
    }
    return vn;
}

ll labs(const ll &x) {
    return x < 0 ? -x : x;
}

void update(ll &x, const ll &y) {
    ll a = labs(2 * x - tot), b = labs(2 * y - tot);
    if(a > b) x = y;
    else if(a == b) chkmax(x, y);
}

int main() {
    freopen("aria.in", "r", stdin);
    freopen("aria.out", "w", stdout);
    
    while(std::cin >> n) {
        tot = 0;
        for(int i = 1; i <= n; ++i) read(a[i]), tot += a[i];
        int xn = get(1, n >> 1, x), yn = get((n >> 1) + 1, n, y);
        ll ans = 0;
        for(int i = 1, j = yn + 1, k = yn + 1; i <= xn; ++i) {
            while(j > 1 && 2 * (y[j - 1] + x[i]) > tot) --j;
            update(ans, y[j] + x[i]);
            if(j > 1) update(ans, y[j - 1] + x[i]);
            while(k > 1 && 4 * (y[k - 1] + x[i]) > tot) --k;
            update(ans, tot - 2 * (y[k] + x[i]));
            if(k > 1) update(ans, tot - 2 * (y[k - 1] + x[i]));
        }
        std::cout << ans << std::endl;
    }
    
    fclose(stdin);fclose(stdout);
    return 0;
}
