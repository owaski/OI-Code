#include <bits/stdc++.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using namespace std;

const int S = 5;
const int L = 209;

int s[S];
int m[300];
bool trans[S][S][S];
bool f[L][L][S];
int len;
char str[L];
string ans;

int main()
{
#ifndef ONLINE_JUDGE
	freopen("bzoj1055.in", "r", stdin);
	freopen("bzoj1055.out", "w", stdout);
#endif

	m['W'] = 1, m['I'] = 2;
	m['N'] = 3, m['G'] = 4;
	for(int i = 1; i <= 4; ++i)
		cin >> s[i];
	for(int i = 1; i <= 4; ++i)
		for(int j = 1; j <= s[i]; ++j)
		{
			char a, b;
			cin >> a >> b;
			int u = m[a], v = m[b];
			trans[u][v][i] = true;	
		}
	scanf("%s", str+1);
	len = strlen(str+1);
	for(int i = 1; i <= len; ++i)
		f[i][i][m[str[i]]] = true;
	for(int l = 2; l <= len; ++l)
		for(int i = 1; i+l-1 <= len; ++i)
		{
			int j = i+l-1;
			for(int k = 1; k <= 4; ++k)
				for(int a = 1; a <= 4; ++a)
					for(int b = 1; b <= 4; ++b)
						if(trans[a][b][k])
							for(int p = i; p < j; ++p)
								f[i][j][k] |= f[i][p][a]&f[p+1][j][b];
		}
	if(f[1][len][1]) ans += 'W';
	if(f[1][len][2]) ans += 'I';
	if(f[1][len][3]) ans += 'N';
	if(f[1][len][4]) ans += 'G';
	if(!ans.size()) puts("The name is wrong!");
	else cout << ans << endl;

	return 0;
}
