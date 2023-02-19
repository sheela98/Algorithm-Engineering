import time


def compute_pi(num_steps):
    width = 1.0 / num_steps  # width of a rectangle
    csum = 0.0  # for summing up all heights of rectangles

    for i in range(num_steps):
        x = (i + 0.5) * width  # midpoint
        csum = csum + (1.0 / (1.0 + x * x))

    return csum * 4 * width


num_steps = 10000000  # number of rectangles

start_time = time.time()
pi = compute_pi(num_steps)
run_time = time.time() - start_time

print("pi with", num_steps, "steps is",
      pi, "in", "%.6f" % run_time, "seconds\n")
