#include "aligned_allocator.h"
#include <immintrin.h>
#include <iostream>
#include <omp.h>
#include <vector>

// aligned_vector is a 64 byte aligned std::vector
template <class T>
using aligned_vector = std::vector<T, alligned_allocator<T, 64>>;

// multiply each int in the array arr by num
void multiply_by_number(int *arr, int n, int num) {
  for (int i = 0; i < n; ++i) {
    arr[i] *= num;
  }
}

// no speed up in writing ugly not portable functions like this one!!!
void multiply_by_number_avx2(int *arr, int n, int num) {
  // int *arr should be at least 32 byte aligned
  auto *vecs = (__m256i *)arr;
  const auto v_mult = _mm256_set1_epi32(num);
  // stepping vectorwise (each vector contains 8 ints)
  for (int i = 0; i < n / 8; ++i) {
    auto result = _mm256_mullo_epi32(vecs[i], v_mult);
    vecs[i] = result;
  }
}

int main() {
  using namespace std;
  int n = 1000000000;
  // create two memory aligned vectors with each n ints, all ints are 3
  aligned_vector<int> vec1(n, 3);
  aligned_vector<int> vec2(n, 3);

  auto start = omp_get_wtime();
  multiply_by_number(vec1.data(), n, 7);
  cout << "multiply_by_number " << omp_get_wtime() - start << " seconds" << endl;

  start = omp_get_wtime();
  multiply_by_number_avx2(vec2.data(), n, 7);
  cout << "multiply_by_number_avx " << omp_get_wtime() - start << " seconds" << endl;

  if (vec1 != vec2) {
    cerr << "failed!\n";
  }
}
