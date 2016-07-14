#include <bits/stdc++.h>

#define ll long long

#define pii std::pair<int,int>
#define mp std::make_pair
#define fi first
#define se second

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

const int N = 500000;

int n;
struct circle {
    int x, y, r;
    circle() {}
    circle(int _x, int _y, int _r):x(_x), y(_y), r(_r) {}
}r[N + 9];
struct data {
    int x, id, f;
    data() {}
    data(int _x, int _id, int _f):x(_x), id(_id), f(_f) {}
    friend bool operator < (const data &p, const data &q) {
        if(p.x != q.x) return p.x < q.x;
        else if(p.f != q.f) return p.f > q.f;
        else return false;
    }
}p[N * 2 + 9];
int pn;

// check ans when insert and erase
// specially check circles which have same y

struct node {
    int y, id;
    node() {}
    node(int _y, int _id):y(_y), id(_id) {}
    friend bool operator < (const node &p, const node &q) {
        if(p.y != q.y) return p.y < q.y;
        else return p.id < q.id;
    }
};
std::set<node> s;
std::set<pii> vis;
int ans;

ll sqr(int x) {
    return (ll)x * x;
}

int check(int a, int b) {
    if(a > b) std::swap(a, b);
    if(vis.count(mp(a, b))) return 0;
    vis.insert(mp(a, b));
    circle p = r[a], q = r[b];
    if(sqr(p.x - q.x) + sqr(p.y - q.y) == sqr(p.r + q.r)) return 1;
    else return 0;
}

int main() {
    freopen("bubble.in", "r", stdin);
    freopen("bubble.out", "w", stdout);

    read(n);
    for(int i = 1; i <= n; ++i) {
        int x, y, r;
        read(x), read(y), read(r);
        ::r[i] = circle(x, y, r);
        p[++pn] = data(x - r, i, 1);
        p[++pn] = data(x + r, i, -1);
    }
    std::sort(p + 1, p + pn + 1);
    for(int i = 1; i <= pn; ++i)
        if(p[i].f == 1) {
            node cur = node(r[p[i].id].y, p[i].id);
            std::set<node>::iterator t = s.lower_bound(cur);
            if(SZ(s)) {
                if(t != s.end()) ans += check(t->id, p[i].id);
                if(t != s.begin()) --t, ans += check(t->id, p[i].id);
            }
            s.insert(cur);
        }
        else {
            node cur = node(r[p[i].id].y, p[i].id);
            s.erase(cur);
            std::set<node>::iterator t = s.lower_bound(cur);
            if(SZ(s)) {
                int x = -1, y = -1;
                if(t != s.end()) x = t->id;
                if(t != s.begin()) --t, y = t->id;
                if(x != -1 && y != -1) ans += check(x, y);
            }
        }
    printf("%d\n", ans);
	
    fclose(stdin);fclose(stdout);
    return 0;
}
