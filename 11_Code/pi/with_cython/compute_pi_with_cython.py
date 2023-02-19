import time
from compute_pi import compute_pi


num_steps = 10000000  # number of rectangles

start_time = time.time()
pi = compute_pi(num_steps) # 30 times faster
run_time = time.time() - start_time

print("pi with", num_steps, "steps is",
      pi, "in", "%.6f" % run_time, "seconds\n")
