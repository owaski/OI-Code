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
static char out[(int)3e7];
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

const int N = 1e6;

int n, m;
int S[N + 9], T[N + 9];
int nex[N + 9], pos[N + 9], pre[N + 9], pre2[N + 9];;

int ansn, ans[N + 9];

bool equal(int i, int j) {
    if(!pre[i] && pre[j] < j - i + 1) return true;
    else if(pre[i] && pre[j] < j - i + 1) return false;
    else if(!pre[i] && pre[j] >= j - i + 1) return false;
    else return pre[i] == pre[j] - (j - i);
}

bool equal2(int i, int j) {
    if(i > m) return false;
    if(!pre[i] && pre2[j] < j - i + 1) return true;
    else if(pre[i] && pre2[j] < j - i + 1) return false;
    else if(!pre[i] && pre2[j] >= j - i + 1) return false;
    else return pre[i] == pre2[j] - (j - i);
}

int main() {
    freopen("reform.in", "r", stdin);
    freopen("reform.out", "w", stdout);

    int Case, C;
    read(Case), read(C);
    while(Case--) {
        std::fill(pos + 1, pos + C + 1, 0);
        read(n), read(m);
        for(int i = 1; i <= n; ++i) read(S[i]);
        for(int i = 1; i <= m; ++i) read(T[i]);
        for(int i = 1; i <= m; ++i)
            pre[i] = pos[T[i]], pos[T[i]] = i;
        std::fill(pos + 1, pos + C + 1, 0);
        for(int i = 1; i <= n; ++i)
            pre2[i] = pos[S[i]], pos[S[i]] = i;
        for(int i = 2; i <= m; ++i) {
            int j = nex[i - 1];
            while(j && !equal(j + 1, i)) j = nex[j];
            if(equal(j + 1, i)) nex[i] = j + 1;
            else nex[i] = 0;
        }
        ansn = 0;
        for(int i = 1, j = 0; i <= n; ++i) {
            while(j && !equal2(j + 1, i)) j = nex[j];
            if(equal2(j + 1, i)) j++;
            if(j == m) ans[++ansn] = i - m + 1;
        }
        write(ansn), out[outn++] = '\n';
        for(int i = 1; i <= ansn; ++i) write(ans[i]), out[outn++] = i == ansn ? '\n' : ' ';
    }
    if(outn) out[--outn] = 0, puts(out);
	
    fclose(stdin);fclose(stdout);
    return 0;
}
