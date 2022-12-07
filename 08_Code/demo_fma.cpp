#include <immintrin.h>
#include <iostream>

int main() {
  // initialize float vectors
  __m256 a = _mm256_setr_ps(1, 2, 3, 4, 5, 6, 7, 8);
  __m256 b = _mm256_setr_ps(2, 3, 4, 5, 6, 7, 8, 9);
  __m256 c = _mm256_setr_ps(1, 2, 1, 2, 1, 2, 1, 2);

  // compute fused-multiply-add with vectors
  auto dst = _mm256_fmadd_ps(a, b, c);

  // show results --> 3 8 13 22 31 44 57 74 
  auto arr = (float *)&dst;
  for (int i = 0; i < 8; ++i)
    std::cout << arr[i] << " ";
}
