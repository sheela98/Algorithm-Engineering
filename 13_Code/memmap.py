import numpy as np

# Attention, here 16*10^9 bytes are written and then read! Can take a long time!
# create a float64 200000 * 10000 matrix (16*10^9 bytes) in working directory
A = np.memmap('A.memmap', dtype='float64', mode='w+', shape=(200000, 10000))

# fill matrix with ones
A.fill(1)

# print sum of all entries in A (--> 2000000000.0)
print(np.sum(A))

# flush all changes in the array (guarantees that the data is actually on disc)
A.flush()

# see here for more information about memory-mapped files in NumPy
# https://numpy.org/doc/stable/reference/generated/numpy.memmap.html#numpy.memmap
