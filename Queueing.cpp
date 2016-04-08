#include <bits/stdc++.h>

const int N = 1000;

double f[N+9][N+9];

struct Queueing {
    inline double probFirst(int l1, int l2, int _p1, int _p2) {
        double p1 = 1.0/_p1, p2 = 1.0/_p2, q1 = 1-p1, q2 = 1-p2;
        for(int i = 1; i <= l2; ++i) f[0][i] = 1;
        for(int i = 1; i <= l1; ++i)
            for(int j = 1; j <= l2; ++j) {
                f[i][j] += f[i-1][j]*p1*q2;
                f[i][j] += f[i][j-1]*q1*p2;
                f[i][j] += f[i-1][j-1]*p1*p2;
                f[i][j] /= 1-q1*q2;
            }
        return f[l1][l2];
    }
};
