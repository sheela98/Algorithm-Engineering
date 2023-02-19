#include <immintrin.h>
#include <iostream>

/*
 * You need to install perf first
 *
 * then see https://github.com/viktorleis/perfevent
 * for installation and usage of perfEvent
 *
 * You may need to run "sudo sysctl -w kernel.perf_event_paranoid=-1"
 * and/or add "kernel.perf_event_paranoid = -1" to /etc/sysctl.conf
 */
#include "PerfEvent.hpp"

using namespace std;

double sum_matrix_columnwise(const double *mat, int64_t n) {
  double result = 0;
  for (int64_t i = 0; i < n; ++i) {
    for (int64_t j = 0; j < n; ++j) {
      result += mat[i + j * n];
    }
  }
  volatile double trigger = result;
  return trigger;
}

double sum_matrix_rowwise(const double *mat, int64_t n) {
  double result = 0;
  for (int64_t i = 0; i < n; ++i) {
    for (int64_t j = 0; j < n; ++j) {
      result += mat[i * n + j];
    }
  }
  volatile double trigger = result;
  return trigger;
}

int main() {
  int64_t n = 20000;
  auto mat = (double *)_mm_malloc(n * n * sizeof(double), 128);
  for (int64_t i = 0; i < n * n; ++i) {
    mat[i] = 1.0;
  }

  BenchmarkParameters params;

  {
    params.setParam("_name", "columnwise"); // set parameter
    PerfEventBlock e(n * n, params, true); // start counter
    sum_matrix_columnwise(mat, n); // computation to benchmark
  }

  {
    params.setParam("_name", "   rowwise"); // change parameter
    PerfEventBlock e(n * n, params, false); // start counter
    sum_matrix_rowwise(mat, n); // computation to benchmark
  }

  _mm_free(mat);
}
