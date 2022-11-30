#include "aligned_allocator.h"
#include <iostream>
#include <omp.h>
#include <vector>

// aligned_vector is a 64 byte aligned std::vector
template <class T>
using aligned_vector = std::vector<T, alligned_allocator<T, 64>>;

// sum up all values in arr and return sum
double sum(const double *arr, int n) {
  double result = 0;
  for (int i = 0; i < n; i++) {
    result = result + arr[i];
  }
  return result;
}

// sum up all values in arr and return sum, use simd hint
double sum_with_simd_hint(const double *arr, int n) {
  double result = 0;
#pragma omp simd reduction(+ : result) // simd in OpenMP
  for (int i = 0; i < n; i++) {
    result = result + arr[i];
  }
  return result;
}

int main() {
  using namespace std;
  int n = 500000000;
  // create two memory aligned vectors with each n doubles, all doubles are 1.0
  aligned_vector<double> a(n, 1.0);
  aligned_vector<double> b(n, 1.0);

  auto start = omp_get_wtime();
  auto result1 = sum(a.data(), n);
  cout << "sum: " << omp_get_wtime() - start << " seconds" << endl;

  start = omp_get_wtime();
  auto result2 = sum_with_simd_hint(b.data(), n);
  cout << "sum with hint: " << omp_get_wtime() - start << " seconds" << endl;

  if (abs(result1 - result2) > 1e-9) {
    cout << "failed!!!" << endl;
  }
}
