#include <immintrin.h>
#include <iostream>

int main() {
  // initialize float vector
  __m256 v = _mm256_setr_ps(1, 2, 3, 4, 5, 6, 7, 8);

  // shuffle float vector v into a new vector result
  // 1, 2, 3, 4, 5, 6, 7, 8 -->  2, 1, 4, 3, 6, 5, 8, 7
  __m256 result = _mm256_shuffle_ps(v, v, 0b10110001); 
  // _mm256_shuffle_ps (latency: 1, throughput: 1)
  // is one of the most difficult intrinsic functions to understand
  // we could also do the same shuffle with:
  // _mm256_permutevar_ps(v, _mm256_setr_epi32(1, 0, 3, 2, 1, 0, 3, 2));

  // show results
  auto arr = (float *)&result;
  for (int i = 0; i < 8; ++i)
    std::cout << arr[i] << " ";
}
