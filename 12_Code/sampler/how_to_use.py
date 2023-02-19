import pyximport
pyximport.install()
# you can also compile the module with cythonize -i DSampling.pyx
# in that case you don't need pyximport

from DSampling import Sampler
import numpy as np

# given the following probabilities, we sample the corresponding indices
probabilities = np.array([0.2, 0.1, 0.3, 0.4])
# normalize --> sum of all probabilities = 1.0, sampler throws error when that's not the case
probabilities = probabilities / sum(probabilities) 

# provide seed 0 -- 2^32-1, default seed is 0
# here, seed is random
seed = np.random.randint(2**32 - 1, dtype=np.uintc)

# construct the Sampler
sampler = Sampler(probabilities, seed)

# single sample
print("single sample:", sampler.sample())
# you sample the index, for example if sample is 2
# then the thing with 0.3 probability was sampled

# a faster way is to fill an integer array with samples
arr = np.zeros(10, dtype=np.intc)
# fill the array with samples
sampler.fill_with_samples(arr)
print("samples in array:", arr)
# you can also fill just a part of an array with samples through slicing
# for example: sampler.fill_with_samples(arr[:2])
