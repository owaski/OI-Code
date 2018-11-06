import numpy as np
import math

R_arr = [1, 5, 10, 50, 100, 250, 500, 1000]

def calc(R, k) :
    rho = R / k
    pi0 = np.longdouble(0)
    tmp = np.longdouble(1)
    for i in range(k) :
        pi0 += tmp
        tmp *= k * rho / (i + 1)
    pi0 += tmp / (1 - rho)
    pi0 = 1 / pi0
    PQ = tmp * pi0 / (1 - rho)
    return PQ

for R in R_arr :
    for k in range(R + 1, R * 2 + 10) :
        if calc(R, k) < 0.2 :
            print("%d : %d" %  (R, k))
            break
