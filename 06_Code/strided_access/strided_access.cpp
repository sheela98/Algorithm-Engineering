#include "aligned_allocator.h"
#include <iostream>
#include <omp.h>
#include <vector>

// aligned_vector is a 64 byte aligned std::vector
template <class T>
using aligned_vector = std::vector<T, alligned_allocator<T, 64>>;

// sum up every eighth element of arr
double sum1(const double *arr, int n) {
  double result = 0;
  for (int i = 0; i < n; i += 8) { // increment by 8
    result = result + arr[i];
  }
  return result;
}

// sum up all elements of arr
// using -Ofast -ffast-math compile flag --> compiler vectorizes code
double sum2(const double *arr, int n) {
  double result = 0;
  for (int i = 0; i < n; ++i) { // increment by 1
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
  auto result1 = sum1(a.data(), n);
  cout << "sum every eighth element: " << omp_get_wtime() - start << " seconds"
       << endl;

  start = omp_get_wtime();
  auto result2 = sum2(b.data(), n);
  cout << "sum every all elements: " << omp_get_wtime() - start << " seconds"
       << endl;

  if(abs(result1 - result2 / 8) > 1e-9){
    cout << "failed!!!" << endl;
  }
}
