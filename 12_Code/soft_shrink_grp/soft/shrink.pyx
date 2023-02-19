# distutils: extra_compile_args = XCOMPARGS
# distutils: extra_link_args = XLINKARGS
# cython: language_level = 3
# cython: boundscheck = False
# cython: wraparound = False
# cython: cdivision = True
# cython: embedsignature = True

import numpy as np

from cython.parallel cimport prange

def grp(double[:,::1] z, double tau, long[:] Lcum, long n_threads = 4):
    """ This function computes ... <give here a description> """
    cdef double shrinkednorm = 0.0, gnorm, num, fac
    cdef int i, j, k;

    for i in prange(Lcum.shape[0] - 1, nogil=True, schedule='static', num_threads=n_threads):
        for j in range(z.shape[1]):
            gnorm = 0.0
            for k in range(Lcum[i], Lcum[i + 1]):
                gnorm += z[k, j] * z[k, j]
            if gnorm > 0.0:
                gnorm = gnorm ** 0.5
                fac = max(0.0, (1.0 - tau / gnorm))
                for k in range(Lcum[i], Lcum[i + 1]):
                    z[k, j] = z[k, j] *  fac
                shrinkednorm += fac * gnorm

    return np.asarray(z), shrinkednorm
