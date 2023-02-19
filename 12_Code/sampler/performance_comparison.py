import pyximport
pyximport.install()

import time
from DSampling import Sampler
import numpy as np

# distribution of letters in alphabet, https://en.wikipedia.org/wiki/Letter_frequency
probabilities = np.array([8.167, 1.492, 2.202, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 1.292, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.356, 2.758, 0.978, 2.560, 0.150, 1.994, 0.077])
# normalize --> sum of all probabilities = 1.0, sampler throws error when that's not the case
probabilities = probabilities / sum(probabilities) 

# provide seed 0 -- 2^32-1, default seed is 0
# here, seed is random
seed = np.random.randint(2**32 - 1, dtype=np.uintc)

# construct the Sampler
sampler = Sampler(probabilities, seed)

#### performance measurement ####
n = 1000000  # amount of samples
arr = np.zeros(n, dtype=np.intc)
start_time = time.time()
sampler.fill_with_samples(arr)
run_time = time.time() - start_time
print("n =", n)
print("fill array with samples", "in", "%.4f" % run_time, "seconds")

start_time = time.time()
for _ in range(n):
    sampler.sample()
run_time = time.time() - start_time
print("sampling by one cython", "in", "%.4f" % run_time, "seconds")

# sampling with python only
class Py_Sampler:
    def __init__(self, probs):
        self.cprobs = np.copy(probs)
        for i in range(1, probs.shape[0]):
            self.cprobs[i] = self.cprobs[i - 1] + self.cprobs[i]

    def sample(self):
        random_double = np.random.random()
        for i in range(self.cprobs.shape[0] - 1):
            if(random_double < self.cprobs[i]):
                return i
        return self.cprobs.shape[0] - 1

# python sampler is significantly slower then the cython sampler
py_sampler = Py_Sampler(probabilities)
start_time = time.time()
for _ in range(n):
    py_sampler.sample()
run_time = time.time() - start_time
print("sampling by one python", "in", "%.4f" % run_time, "seconds")
