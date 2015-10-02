#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1009;
const double pi = acos(-1.0);

struct Pot
{
	double x, y;
	Pot(double x = 0, double y = 0):x(x), y(y) {}
	inline void read()
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
	inline friend double operator * (const Pot &a, const Pot &b)
	{
		return a.x*b.x+a.y*b.y;
	}
	inline friend double operator ^ (const Pot &a, const Pot &b)
	{	
		return a.x*b.y-a.y*b.x;
	}
	inline friend Pot operator * (const Pot &a, double k)
	{
		return Pot(a.x*k, a.y*k);	
	}
	inline friend Pot operator / (const Pot &a, double k)
	{
		return Pot(a.x/k, a.y/k);	
	}
	inline void rotate(double ang)
	{
		double sina = sin(ang), cosa = cos(ang);
		*this = Pot(x*cosa-y*sina, x*sina+y*cosa);
	}
};

int n;
Pot o[MAXN];
double r[MAXN];	
vector<pair<double,double> > cov[MAXN];
double ans;

template<class T> inline T sqr(const T &x)
{
	return x*x;
}

inline double dis2(const Pot &a, const Pot &b)
{
	return sqr(a.x-b.x)+sqr(a.y-b.y);
}

inline bool cross(int i, int j)
{
	return dis2(o[i], o[j]) < sqr(r[i]+r[j]);
}

inline void adjust(double &x)
{
	while(x > pi) x -= 2*pi;
	while(x < -pi) x += 2*pi;	
}

inline void get_cov(int i, int j)
{
	double r1 = r[i], r2 = r[j];
	Pot o2 = o[j]-o[i];
	double ang = atan2(o2.y, o2.x);
	o2.rotate(-ang);
	double x = (sqr(r1)-sqr(r2))/(2*o2.x)+o2.x/2;
	double y = sqrt(sqr(r1)-sqr(x));
	double a1 = atan2(y, x), a2 = atan2(-y, x);
	a1 += ang, a2 += ang, adjust(a1), adjust(a2);
	if(a2 < a1) cov[i].push_back(make_pair(a2, a1));
	else
	{
		cov[i].push_back(make_pair(a2, pi));
		cov[i].push_back(make_pair(-pi, a1));	
	}
}

int main()
{
	freopen("bzoj1043.in", "r", stdin);
	freopen("bzoj1043.out", "w", stdout);
	
	scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		scanf("%lf", r+i);
		o[i].read();	
	}
	for(int i = 1; i <= n; ++i)
		for(int j = i+1; j <= n; ++j)
			if(cross(i, j))
			{
				if(dis2(o[i], o[j]) > sqr(r[i]-r[j]))
					get_cov(i, j);
				else if(r[i] <= r[j])
					cov[i].push_back(make_pair(-pi, pi));
			}
	for(int i = 1; i <= n; ++i)
	{
		sort(cov[i].begin(), cov[i].end());
		double le = -pi, ri = -pi, len = 0;
		for(int j = 0; j < (int)cov[i].size(); ++j)
			if(cov[i][j].first > ri)
			{
				le = cov[i][j].first;
				ri = cov[i][j].second;
				len += ri-le;
			}
			else if(cov[i][j].second > ri)
			{
				len += cov[i][j].second-ri;
				ri = cov[i][j].second;
			}
		ans += (2*pi-len)*r[i];
	}
	printf("%.3lf\n", ans);
	
	return 0;
}
