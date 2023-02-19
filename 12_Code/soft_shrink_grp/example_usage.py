from soft.shrink import grp
import numpy as np

rows = 4000
columns = 8000

z = np.random.rand(rows, columns)
tau = 0.5
Lcum = np.array(list(range(0, rows, 8)))

zr, shrinkednorm = grp(z, tau, Lcum)

print(shrinkednorm)
