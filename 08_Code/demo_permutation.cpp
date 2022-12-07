#include <immintrin.h>
#include <iostream>

int main() {
  float arr[8] = {1, 2, 3, 4, 5, 6, 7, 8};

  // load (unaligned) array data into vector register
  __m256 v = _mm256_loadu_ps(arr);

  // indices of permutation
  __m256i idx = _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0);
  // reverse vector (we can permute the vector in any order)
  __m256 result = _mm256_permutevar8x32_ps(v, idx);
  // _mm256_permutevar8x32_ps (latency: 3, throughput: 1)

  // store data from vector register into array (unaligned)
  _mm256_storeu_ps(arr, result);

  // show results: 8, 7, 6, 5, 4, 3, 2, 1
  for (float i : arr) std::cout << i << " ";
}
