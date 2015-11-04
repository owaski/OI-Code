#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int N = 2009;

int n;

struct Pot
{
    double x, y;
    Pot() {x = y = 0;}
    Pot(double _x, double _y):x(_x), y(_y) {}
    inline void input()
    {
        scanf("%lf%lf", &x, &y);
    }
    inline friend Pot operator + (const Pot &a, const Pot &b)
    {
        return Pot(a.x+b.x, a.y+b.y);
    }
    inline friend Pot operator - (const Pot &a, const Pot &b)
    {
        return Pot(a.x-b.x, a.y-b.y);
    }
    inline friend Pot operator * (const Pot &a, const double &k)
    {
        return Pot(a.x*k, a.y*k);
    }
    inline friend Pot operator / (const Pot &a, const double &k)
    {
        return Pot(a.x/k, a.y/k);
    }
    inline friend double operator * (const Pot &a, const Pot &b)
    {
        return a.x*b.x+a.y*b.y;
    }
    inline friend double operator ^ (const Pot &a, const Pot &b)
    {
        return a.x*b.y-a.y*b.x;
    }
    inline friend bool operator < (const Pot &a, const Pot &b)
    {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
    inline friend bool operator == (const Pot &a, const Pot &b)
    {
        return a.x == b.x && a.y == b.y;
    }
}p[N], st[N];

int top;

inline void init()
{
    scanf("%d", &n);
    for(int i = 1; i <= n; ++i)
        p[i].input();
}

inline void convexHull()
{
    top = 0;
    sort(p+1, p+n+1);
    n = unique(p+1, p+n+1)-p-1;
    for(int i = 1; i <= n; ++i)
    {
        while(top > 1 && ((st[top]-p[i])^(st[top-1]-p[i])) < 0) --top;
        st[++top] = p[i];
    }
    int tmp = top;
    for(int i = n-1; i >= 1; --i)
    {
        while(top > tmp && ((st[top]-p[i])^(st[top-1]-p[i])) < 0) --top;
        st[++top] = p[i];
    }
    if(top > 1) top--;
}

inline int nex(int id)
{
    if(id < top) return id+1;
    else return 1;
}

inline double area(const Pot &a, const Pot &x, const Pot &y)
{
    return fabs((x-a)^(y-a))*0.5;
}

inline void solve()
{
    convexHull();
    double ans = 0;
    for(int i = 1; i <= top; ++i)
    {
        int a = nex(i), j = nex(a), b = nex(j);
        while(nex(j) != i)
        {
			if(j == b) b = nex(b);
            while(nex(a) != j && area(st[a+1], st[i], st[j]) >= area(st[a], st[i], st[j])) a = nex(a);
            while(nex(b) != i && area(st[b+1], st[i], st[j]) >= area(st[b], st[i], st[j])) b = nex(b);
            ans = max(ans, area(st[a], st[i], st[j])+area(st[b], st[i], st[j]));
            j = nex(j);
        }
    }
    printf("%.3lf\n", ans);    
}

int main()
{
    init();
    solve();
    return 0;
}
