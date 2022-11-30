#include "aligned_allocator.h"
#include <algorithm>
#include <iostream>
#include <omp.h>
#include <vector>

// aligned_vector is a 64 byte aligned std::vector
template <class T>
using aligned_vector = std::vector<T, alligned_allocator<T, 64>>;

// find min in array a under the condition a[i] > threshold
int find_min_std(int *a, int n, int threshold) {
  int result = INT32_MAX;
  for (int i = 0; i < n; i++) {
    if (a[i] > threshold)
      result = std::min(a[i], result);
  }
  return result;
}

// "omp declare simd" tells the compiler to create several versions
// of the function, some with vector instructions and one without
// clang shows message that it can't create the simd versions!
#pragma omp declare simd
int my_min(int a, int b) { return a < b ? a : b; }

// find min in array a under the condition that a[i] > threshold
int find_min(int *a, int n, int threshold) {
  int result = INT32_MAX;
#pragma omp simd reduction(min : result) // min reduction
  for (int i = 0; i < n; i++) {
    bool is_greater = a[i] > threshold;
    result = my_min(a[i], result) * is_greater + !is_greater * result;
  }
  return result;
}

int main() {
  using namespace std;
  int n = 500000000;
  // create two memory aligned vectors with each n doubles, all doubles are 1.0
  std::vector<int> a(n);
  // quickly, create bad random numbers...
  std::generate(a.begin(), a.end(), rand);
  int num = rand(); // bad random number generator!! don't use it

  auto start = omp_get_wtime();
  int result1 = find_min_std(a.data(), n, num);
  cout << "no hints, min: " << omp_get_wtime() - start << " seconds" << endl;

  start = omp_get_wtime();
  int result2 = find_min(a.data(), n, num);
  cout << "simd hints, min: " << omp_get_wtime() - start << " seconds" << endl;

  if (result1 != result2) {
    cout << "failed!!!" << endl;
  }
}
