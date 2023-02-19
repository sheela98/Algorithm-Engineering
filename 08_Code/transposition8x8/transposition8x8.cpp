#include "aligned_allocator.h"
#include <immintrin.h>
#include <iostream>
#include <omp.h>
#include <vector>

template <class T>
using aligned_vector = std::vector<T, alligned_allocator<T, 64>>;

inline void transpose_8x8_int_naive(int *a) {
  for (int i = 0; i < 8; ++i) {
    for (int j = i + 1; j < 8; ++j) {
      std::swap(a[i * 8 + j], a[j * 8 + i]);
    }
  }
}

// transpose 8 x 8 matrix with intrinsics
inline void transpose_8x8_int_intrinsics(__m256i *vecs) {
  __m256 *v = reinterpret_cast<__m256 *>(vecs);
  __m256 a = _mm256_unpacklo_ps(v[0], v[1]);
  __m256 b = _mm256_unpackhi_ps(v[0], v[1]);
  __m256 c = _mm256_unpacklo_ps(v[2], v[3]);
  __m256 d = _mm256_unpackhi_ps(v[2], v[3]);
  __m256 e = _mm256_unpacklo_ps(v[4], v[5]);
  __m256 f = _mm256_unpackhi_ps(v[4], v[5]);
  __m256 g = _mm256_unpacklo_ps(v[6], v[7]);
  __m256 h = _mm256_unpackhi_ps(v[6], v[7]);
  auto tmp = _mm256_shuffle_ps(a, c, 0x4E);
  a = _mm256_blend_ps(a, tmp, 0xCC);
  c = _mm256_blend_ps(c, tmp, 0x33);
  tmp = _mm256_shuffle_ps(b, d, 0x4E);
  b = _mm256_blend_ps(b, tmp, 0xCC);
  d = _mm256_blend_ps(d, tmp, 0x33);
  tmp = _mm256_shuffle_ps(e, g, 0x4E);
  e = _mm256_blend_ps(e, tmp, 0xCC);
  g = _mm256_blend_ps(g, tmp, 0x33);
  tmp = _mm256_shuffle_ps(f, h, 0x4E);
  f = _mm256_blend_ps(f, tmp, 0xCC);
  h = _mm256_blend_ps(h, tmp, 0x33);
  v[0] = _mm256_permute2f128_ps(a, e, 0x20);
  v[1] = _mm256_permute2f128_ps(c, g, 0x20);
  v[2] = _mm256_permute2f128_ps(b, f, 0x20);
  v[3] = _mm256_permute2f128_ps(d, h, 0x20);
  v[4] = _mm256_permute2f128_ps(a, e, 0x31);
  v[5] = _mm256_permute2f128_ps(c, g, 0x31);
  v[6] = _mm256_permute2f128_ps(b, f, 0x31);
  v[7] = _mm256_permute2f128_ps(d, h, 0x31);
}

void test_correctness() {
  aligned_vector<float> a(64);
  for (int i = 0; i < 64; ++i) {
    a[i] = i;
  }
  auto b = a;
  transpose_8x8_int_naive((int *)a.data());
  transpose_8x8_int_intrinsics((__m256i *)b.data());
  if (a != b) {
    std::cerr << "failed!" << std::endl;
  }
}

int main() {
  using namespace std;
  test_correctness();
  int iterations = 100000000;
  aligned_vector<float> a(64);
  for (int i = 0; i < 64; ++i) {
    a[i] = i;
  }
  auto b = a;

  { // clang optimizes computation away!
    auto start = omp_get_wtime();
    for (int i = 0; i < iterations; ++i) {
      transpose_8x8_int_naive((int *)a.data());
    };
    cout << "transpose 8 x 8 matrix (naive): " << omp_get_wtime() - start
         << " seconds" << endl;
  }

  {
    auto start = omp_get_wtime();
    for (int i = 0; i < iterations; ++i) {
      transpose_8x8_int_intrinsics((__m256i *)b.data());
    };
    cout << "transpose 8 x 8 matrix (intrinsics): " << omp_get_wtime() - start
         << " seconds" << endl;
  }

  if(a != b){
    cerr << "failed!";
  }
}
