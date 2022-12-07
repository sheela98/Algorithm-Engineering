#include "aligned_allocator.h"
#include <iostream>
#include <omp.h>
#include <vector>

// aligned_vector is a 64 byte aligned std::vector
template <class T>
using aligned_vector = std::vector<T, alligned_allocator<T, 64>>;
//using aligned_vector = std::vector<T>;

int main() {
  using namespace std;
  int n = 1000;
  int iterations = 10000000;

  aligned_vector<float> a_vec(n, 1.1f);
  aligned_vector<float> b_vec(n, 1.2f);
  float* a = a_vec.data(); // a is 64-byte aligned
  float* b = b_vec.data(); // b is 64-byte aligned
  float s = 1.3f;

  auto start = omp_get_wtime();
  while(iterations --> 0) {
#pragma omp simd aligned(a, b: 64)
    for (int i = 0; i < n; i++) {
      a[i] += s * b[i];
    }
  };
  cout << omp_get_wtime() - start << " seconds" << endl;
}
