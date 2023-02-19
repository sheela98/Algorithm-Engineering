# cython: language_level = 3

from libc.stdint cimport uint64_t


def compute_pi(uint64_t num_steps):
    cdef double width = 1.0 / num_steps  # width of a rectangle
    cdef double csum = 0.0  # for summing up all heights of rectangles

    cdef uint64_t i
    cdef double x
    for i in range(num_steps):
        x = (i + 0.5) * width  # midpoint
        csum = csum + (1.0 / (1.0 + x * x))

    return csum * 4 * width
