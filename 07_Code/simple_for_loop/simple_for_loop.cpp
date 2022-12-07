#include "aligned_allocator.h"
#include <immintrin.h>
#include <iostream>
#include <omp.h>
#include <vector>

// aligned_vector is a 64 byte aligned std::vector
template <class T>
using aligned_vector = std::vector<T, alligned_allocator<T, 64>>;
//using aligned_vector = std::vector<T>;

// pointers are not restricted
void dummy_func_slow(int *a, int *b, const int *c, int n) {
  for (int i = 0; i < n - 1; ++i) {
    a[i] += b[i]; // b[i] is dependent on previous b[i + 1]
    b[i + 1] += c[i];
  }
}

void dummy_func_fast(int *__restrict__ a, int *__restrict__ b,
                     const int *__restrict__ c, int n) {
#pragma omp simd
  for (int i = 0; i < n - 1; ++i) {
    b[i + 1] += c[i];
  }
#pragma omp simd
  for (int i = 0; i < n - 1; ++i) {
    a[i] += b[i];
  }
}

void dummy_func_fast2(int *__restrict__ a, int *__restrict__ b,
                      const int *__restrict__ c, int n) {
  a[0] += b[0];
#pragma omp simd aligned(a, b, c: 64)
  for (int i = 1; i < n - 1; ++i) {
    b[i] += c[i - 1];
    a[i] += b[i];
  }
  b[n - 1] += c[n - 2];
}

int main() {
  using namespace std;
  int n = 1000;
  int iterations = 1000000;

  aligned_vector<int> a(n, 1);
  aligned_vector<int> b(n, 2);
  aligned_vector<int> c(n, 3);
  auto start = omp_get_wtime();
  // don't care here with integer overflow, just dummy computation
  for (int i = 0; i < iterations; ++i) {
    dummy_func_slow(a.data(), b.data(), c.data(), n);
  };
  cout << "dummy slow: " << omp_get_wtime() - start << " seconds" << endl;

  aligned_vector<int> a2(n, 1);
  aligned_vector<int> b2(n, 2);
  aligned_vector<int> c2(n, 3);
  start = omp_get_wtime();
  // don't care here with integer overflow, just dummy computation
  for (int i = 0; i < iterations; ++i) {
    dummy_func_fast(a2.data(), b2.data(), c2.data(), n);
  };
  cout << "dummy fast: " << omp_get_wtime() - start << " seconds" << endl;

  aligned_vector<int> a3(n, 1);
  aligned_vector<int> b3(n, 2);
  aligned_vector<int> c3(n, 3);
  start = omp_get_wtime();
  // don't care here with integer overflow, just dummy computation
  for (int i = 0; i < iterations; ++i) {
    dummy_func_fast2(a3.data(), b3.data(), c3.data(), n);
  };
  cout << "dummy fast2: " << omp_get_wtime() - start << " seconds" << endl;

  if (a != a2 || b != b2 || c != c2 || a != a3 || b != b3 || c != c3) {
    cerr << "failed!\n";
  }
}
