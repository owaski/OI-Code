#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int S = 13000;
const int INF = 0x3f3f3f3f;

namespace Trans
{
	int a[4][4];
	int status[100000], tot;
	int step[100000];
	int start, end;
	int d[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
	inline void input()
	{
		start = 0;
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
			{
				scanf("%1d", &a[i][j]);	
				start |= a[i][j]<<(i*4+j);
			}
		status[start] = ++tot;
	}
	inline void extend()
	{
		queue<int> q;
		q.push(start);
		while(!q.empty())
		{
			int now = q.front(), to = now;q.pop();
			for(int i = 0; i < 4; ++i)
				for(int j = 0; j < 4; ++j)
					a[i][j] = (now>>(i*4+j))&1;
			for(int i = 0; i < 4; ++i)
				for(int j = 0; j < 4; ++j)
					if(a[i][j])
						for(int k = 0; k < 4; ++k)
						{
							int ti = i+d[k][0];
							int tj = j+d[k][1];
							if(ti >= 0 && ti < 4)
								if(tj >= 0 && tj < 4)
									if(!a[ti][tj])
									{
										to = now;
										to ^= 1<<(i*4+j);
										to ^= 1<<(ti*4+tj);
										if(!status[to])
										{
											step[to] = step[now]+1;
											status[to] = ++tot;
											q.push(to);	
										}
									}
						}
		}
	}
	inline void solve()
	{
		end = 0;
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
			{
				scanf("%1d", &a[i][j]);	
				end |= a[i][j]<<(i*4+j);
			}
		cout << step[end] << endl;
	}
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1054.in", "r", stdin);
	freopen("bzoj1054.out", "w", stdout);
#endif

	Trans::input();
	Trans::extend();
	Trans::solve();

	return 0;
}
