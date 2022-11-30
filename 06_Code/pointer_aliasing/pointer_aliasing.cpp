#include "aligned_allocator.h"
#include <iostream>
#include <memory>
#include <omp.h>
#include <vector>

// aligned_vector is a 64 byte aligned std::vector
template <class T>
using aligned_vector = std::vector<T, alligned_allocator<T, 64>>;

// sum all values in arr to *dest
void sum_all(const int *arr, int *dest, int n) {
  *dest = 0.0;
  for (int i = 0; i < n; i++) {
    *dest = *dest + arr[i]; // arr and dest may point to same value
  }
}

// restricted pointers signal a non-overlapping memory location
void sum_all_not_aliased(const int *__restrict__ arr,
                               int *__restrict__ dest, int n) {
  *dest = 0.0; // we could also create a temporary variable "tmp_dest"
  for (int i = 0; i < n; i++) {
    // arr and dest are not allowed to point to the same value
    *dest = *dest + arr[i];
  }
}

// sum all values in arr to *dest, use local variable
void sum_all_not_aliased2(const int *arr, int *dest, int n) {
  int tmp_dest = 0.0; // local variable for summation
  // #pragma omp simd aligned(arr: 64) // may speed up further
  for (int i = 0; i < n; i++) {
    tmp_dest = tmp_dest + arr[i];
  }
  *dest = tmp_dest;
}

int main() {
  using namespace std;
  int _n = 96;
  // allocating on the heap, prevent compiler from optimizing possible pointer
  // aliasing away
  auto n = std::unique_ptr<int>(new int(_n));
  auto result = std::unique_ptr<int>(new int(0));
  // create one memory aligned vector with n ints, all ints are 1
  aligned_vector<int> a(*n, 1);

  auto start = omp_get_wtime();
  long final_result = 0;
  long iterations = 100000000;
  for (long i = 0; i < iterations; ++i) {
    sum_all(a.data(), result.get(), *n);
    final_result += *result;
  }
  cout << "possible pointer aliasing: " << omp_get_wtime() - start << " seconds"
       << endl;

  start = omp_get_wtime();
  for (long i = 0; i < iterations; ++i) {
    sum_all_not_aliased(a.data(), result.get(), *n);
    final_result += *result;
  }
  cout << "no pointer aliasing: " << omp_get_wtime() - start << " seconds"
       << endl;

  if ((*n * iterations) * 2 != final_result) {
    cout << "failed!!!\n";
  }
}
