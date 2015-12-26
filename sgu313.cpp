#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)

const int N = 200009;
const long long INF = 1ll<<60;

int n, L;

struct Data {
    int type, x, id;

    Data() {type = x = id = 0;}
    Data(int _type, int _x, int _id):type(_type), x(_x), id(_id) {}

    inline friend bool operator < (const Data &p, const Data &q) {
        return p.x < q.x;
    }
}p[N], st[N];
int p_n;

struct Data2 {
    int len, cnt, id;

    Data2() {len = cnt = id = 0;}
    Data2(int _len, int _cnt, int _id):len(_len), cnt(_cnt), id(_id) {}

    inline friend bool operator < (const Data2 &p, const Data2 &q) {
        return p.cnt < q.cnt;
    }
}info[N];

long long cut;
long long ans = INF;
long long sum;
long long cnt;

int match[N];

inline void init() {
    scanf("%d%d", &n, &L);
    for(int i = 1; i <= n; ++i) {
        int x;scanf("%d", &x);
        p[++p_n] = Data(1, x, i);
    }
    for(int i = 1; i <= n; ++i) {
        int x;scanf("%d", &x);
        p[++p_n] = Data(2, x, i);
    }
}

inline int nex(int x) {
    return x==p_n ? 1 : x+1;
}

inline void get_match(int s) {
    int i = s, top = 0;
    do {
        if(!top || st[top].type == p[i].type) st[++top] = p[i];
        else {
            if(p[i].type == 1) match[p[i].id] = st[top].id;
            else match[st[top].id] = p[i].id;top--;
        }
        i = nex(i);
    }while(i != s);
}

inline int abs(int x) {
    return x<0 ? -x : x;
}

inline void solve() {
    std::sort(p+1, p+p_n+1), p[p_n+1].x = p[1].x+L;
    for(int i = 1; i <= p_n; ++i) {
        info[i] = Data2(p[i+1].x-p[i].x, info[i-1].cnt+(p[i].type==1?-1:1), i);
        //debug("%d\n", info[i].cnt);
    }
    std::sort(info+1, info+p_n+1);
    for(int i = 1; i <= p_n; ++i)                    
        sum += 1ll*info[i].len*(info[i].cnt-info[1].cnt);
    ans = sum, cut = info[1].id, cnt = info[1].len;
    //debug("%lld\n", sum);
    for(int i = 2; i <= p_n; ++i) {
        sum += (2*cnt-L)*(info[i].cnt-info[i-1].cnt);
        //debug("%lld\n", sum);
        if(sum < ans) ans = sum, cut = info[i].id;
        cnt += info[i].len;
    }
    get_match(nex(cut));
    std::cout << ans << std::endl;
    for(int i = 1; i <= n; ++i)
        printf("%d%c", match[i], i==n?'\n':' ');
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    init();
    solve();
    
#ifndef ONLINE_JUDGE
    fclose(stdin);fclose(stdout);
#endif
    return 0;
}
