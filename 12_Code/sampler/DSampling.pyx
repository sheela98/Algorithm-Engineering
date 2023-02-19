# cython: language_level = 3
# cython: boundscheck = False
# cython: wraparound = False
# cython: embedsignature=True

from libc.stdint cimport uint64_t, uint32_t
from libc.stdlib cimport malloc, free
from libc.stdlib cimport qsort

cdef inline uint64_t rotl(uint64_t x, int k):
    return (x << k) | (x >> (64 - k))

cdef inline uint64_t next_rand(uint64_t* s0, uint64_t* s1):
    s1[0] = s1[0] ^ s0[0]
    s0[0] = rotl(s0[0], 24) ^ s1[0] ^ (s1[0] << 16)
    s1[0] = rotl(s1[0], 37)
    return s0[0] + s1[0]

cdef inline double next_double_rand(uint64_t* s0, uint64_t* s1):
    cdef uint64_t n = (next_rand(s0, s1) >> 12) | 0x3FF0000000000000
    cdef double* num = <double*>&n
    return num[0] - 1.0
    
cdef class Rand:
    cdef:
        uint64_t s1 
        uint64_t s2
        
    def __init__(self, uint32_t seed = 0):
        self.s1 = 1314472907419283471ull + <uint64_t>seed # seed 1
        self.s2 = 7870872464127966567ull + <uint64_t>seed # seed 2
        cdef int i
        for i in range(10): # warm up
            self.next_double()
        
    cpdef double next_double(self):
        return next_double_rand(&self.s1, &self.s2)
     
cdef struct Item:
    double key
    int index
    
cdef int comparator(const void* pa, const void* pb) nogil:
    if (<Item*>pa)[0].key < (<Item*>pb)[0].key:
        return 1
    elif (<Item*>pa)[0].key > (<Item*>pb)[0].key:
        return -1
    else:
        return 0

cdef class Sampler:
    """
    Sampler for discrete distributions.
    """
    cdef:
        Rand generator
        double *cprobs
        int* categories
        int length
    
    def __cinit__(self, double[:] cprobs, uint32_t seed = 0):
        self.length = cprobs.shape[0]
        if(self.length < 2):
            raise Exception("Less than two categories given.")
        cdef int i
        cdef csum = 0.0
        cdef bint less_zero = False
        for i in range(self.length):
            csum = csum + cprobs[i]
            less_zero = less_zero or (cprobs[i] < 0.0)
        if less_zero:
            raise Exception("Negative probabilities are invalid.")
        if (csum < 1.0 - 1e-6) or (csum > 1 + 1e-6):
            raise Exception("Sum of probabilities is " + str(csum) + ". But should be around 1.0.")
        self.cprobs = <double*>malloc(self.length * sizeof(double))
        if self.cprobs == NULL:
            raise MemoryError("Unable to allocate enough memory.")
        self.categories = <int*>malloc(self.length * sizeof(int))
        if self.categories == NULL:
            raise MemoryError("Unable to allocate enough memory.")
        cdef Item* items = <Item*>malloc(self.length * sizeof(Item))
        if items == NULL:
            raise MemoryError("Unable to allocate enough memory.")
        for i in range(self.length):
            items[i].key = cprobs[i]
            items[i].index = i
        qsort(items, self.length, sizeof(Item), comparator)
        for i in range(self.length):
            self.cprobs[i] = items[i].key
            self.categories[i] = items[i].index
        for i in range(1, self.length):
            self.cprobs[i] = self.cprobs[i - 1] + self.cprobs[i]
        free(items)
        self.generator = Rand(seed)
        
    cpdef sample(self):
        cdef int i
        cdef double random_double = self.generator.next_double()
        for i in range(self.length - 1):
            if(random_double < self.cprobs[i]):
                return self.categories[i]     
        return self.categories[self.length - 1]
    
    cpdef fill_with_samples(self, int[:] view):
        cdef int n = view.shape[0], i
        for i in range(n):
            view[i] = self.sample()
        
    def __dealloc__(self):
        free(self.cprobs) 
        free(self.categories) 
