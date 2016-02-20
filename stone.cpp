#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
 
#define ll long long
 
#define debug(...) fprintf(stderr, __VA_ARGS__)
 
template<class T>
inline void read(T &x) {
    char c;int f = 1;x = 0;
    while(((c=getchar()) < '0' || c > '9') && c != '-');
    if(c == '-') f = -1;else x = c-'0';
    while((c=getchar()) >= '0' && c <= '9') x = x*10+c-'0';
    x *= f;
}
 
const int N = 1000000;
 
int n;
 
struct list {
    int head, tail, cnt;
    ll val[N+9];
    int nex[N+9], pre[N+9];
    bool ok[N+9];
    ll ans1, ans2, sum, dec;
     
    list() {
        head = tail = cnt = sum = dec = 0;
        memset(ok, 0, sizeof ok);
        memset(val, 0, sizeof val);
        memset(nex, 0, sizeof nex);
        memset(pre, 0, sizeof pre);
    }
    inline void add(int x, int y) {
        nex[x] = y, pre[y] = x;
    }
     
    inline void init() {
        for(int i = 1; i <= n; ++i)
            read(val[i]), ok[i] = val[i]>0, cnt += val[i]>0, sum += val[i];
        for(int i = 1; i < n; ++i) add(i, i+1);
        head = 1, tail = n;
    }
 
    inline void del(int x) {
        ok[x] = false;
        if(x == head) head = nex[x];
        if(x == tail) tail = pre[x];
        if(pre[x]) nex[pre[x]] = nex[x];
        if(nex[x]) pre[nex[x]] = pre[x];
    }
     
    inline bool work1() {
        bool ret = false;
        while(ok[head] && ok[nex[head]] && val[head] >= val[nex[head]]) {
            if(cnt&1) ans1 += val[head], ans2 += val[nex[head]];
            else ans1 += val[nex[head]], ans2 += val[head];
            del(head), del(head), ret = true;
        }
        while(ok[tail] && ok[pre[tail]] && val[tail] >= val[pre[tail]]) {
            if(cnt&1) ans1 += val[tail], ans2 += val[pre[tail]];
            else ans1 += val[pre[tail]], ans2 += val[tail];
            del(tail), del(tail), ret = true;
        }
        return ret;
    }
 
    inline bool work2() {
        bool ret = false;
        for(int i = nex[head]; nex[i]; i = nex[i])
            if(ok[pre[i]] && ok[i] && ok[nex[i]] && val[i] > val[pre[i]] && val[i] > val[nex[i]]) {
                val[i] = val[pre[i]]+val[nex[i]]-val[i];
                del(pre[i]), del(nex[i]), ret = true;
            }
        return ret;
    }
     
    inline void solve() {
        while(work1() || work2());
        int m = 0;
        for(int i = head; i; i = nex[i])
            if(ok[i]) val[++m] = val[i];
        sum -= ans1+ans2;
        std::sort(val+1, val+m+1, std::greater<ll>());
        for(int i = 1; i <= m; ++i)
            if(i&1) dec += val[i];
            else dec -= val[i];
        std::cout << ans1+(sum+dec)/2 << ' ' << ans2+(sum-dec)/2 << std::endl;
    }
}line;
 
int main() {
    read(n);
    line.init();
    line.solve();
    return 0;
}
