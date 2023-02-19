#include "aligned_allocator.h"
#include <array>
#include <immintrin.h>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <vector>

template<class T>
using aligned_vector = std::vector<T, alligned_allocator<T, 64>>;

using namespace std;

#if defined(_WIN32) // restrict pointers on windows
#if defined(_MSC_VER) || defined(__ICL)
#define __restrict__ __restrict
#endif
#endif

#define PRAGMA(X) _Pragma(#X)
#if defined(__INTEL_COMPILER)
#define __unroll(X) PRAGMA(unroll(X))
#elif defined(__clang__)
#define __unroll(X) PRAGMA(clang loop unroll_count(X))
#elif defined(__GNUC__) || defined(__GNUG__)
#define __unroll(X) PRAGMA(GCC unroll(X))
#else
// do nothing
#define __unroll(X)
// define here __unroll macro for your compiler
#endif

void fma_and_swap(const float *__restrict__ a, const float *__restrict__ b,
                  const float *__restrict__ c, float *__restrict__ d,
                  const int n) {
  for (int i = 0; i < n; i += 2) {
    d[i + 1] = a[i] * b[i] + c[i]; // fma
    d[i] = a[i + 1] * b[i + 1] + c[i + 1]; // fma
  }
}


void fma_and_swap_tuned(const float *__restrict__ a,
                        const float *__restrict__ b,
                        const float *__restrict__ c, float *__restrict__ d,
                        const int n) {
  // slower with two separate for loops
  // __unroll is a custom macro for unrolling loops
  //  __unroll(64) for (int i = 0; i < n; ++i) {
  //    d[i] = a[i] * b[i] + c[i];
  //  }
  //  __unroll(32) for (int i = 0; i < n; i += 2) {
  //    std::swap(d[i], d[i + 1]);
  //  }

  //__unroll is a custom macro for unrolling loops
  __unroll(8) for (int i = 0; i < n; i += 8) {
#pragma omp simd aligned(a, b, c, d : 64)
    for (int j = i; j < i + 8; ++j) { 
      d[j] = a[j] * b[j] + c[j]; // 8 floats (AVX2)
    }
#pragma omp simd aligned(d : 64)
    for (int j = i; j < i + 8; j += 2) {
      std::swap(d[j], d[j + 1]); // 8 floats (AVX2)
    }
  }
}

void fma_and_swap_intrinsics(const float *__restrict__ a,
                             const float *__restrict__ b,
                             const float *__restrict__ c, float *__restrict__ d,
                             const int n) {
  auto av = (__m256 *) a; // a at least 32-byte aligned
  auto bv = (__m256 *) b; // b at least 32-byte aligned
  auto cv = (__m256 *) c; // c at least 32-byte aligned
  auto dv = (__m256 *) d; // d at least 32-byte aligned

  //__unroll is a custom macro for unrolling loops
  __unroll(8) for (int i = 0; i < n / 8; i++) {
    dv[i] = _mm256_fmadd_ps(av[i], bv[i], cv[i]);
    dv[i] = _mm256_shuffle_ps(dv[i], dv[i], 0b10110001);
  }
}

int main() {
  using namespace std;
  // volatile forces compiler to actually compute the results at runtime
  volatile int n = 1000;
  int iterations = 10000000;


  aligned_vector<float> d(n);
  {
    aligned_vector<float> a(n);
    iota(begin(a), end(a), 1.0f);
    aligned_vector<float> b(n, 5);
    aligned_vector<float> c(n, 7);
    auto start = omp_get_wtime();

    for (int i = 0; i < iterations; ++i) {
      fma_and_swap(a.data(), b.data(), c.data(), d.data(), n);
    };
    cout << "fma and swap: " << omp_get_wtime() - start << " seconds" << endl;
  }

  aligned_vector<float> dt(n);
  {
    aligned_vector<float> a(n);
    iota(begin(a), end(a), 1.0f);
    aligned_vector<float> b(n, 5);
    aligned_vector<float> c(n, 7);
    auto start = omp_get_wtime();
    for (int i = 0; i < iterations; ++i) {
      fma_and_swap_tuned(a.data(), b.data(), c.data(), dt.data(), n);
    };
    cout << "fma and swap tuned: " << omp_get_wtime() - start << " seconds"
         << endl;
  }

  aligned_vector<float> di(n);
  {
    aligned_vector<float> a(n);
    iota(begin(a), end(a), 1.0f);
    aligned_vector<float> b(n, 5);
    aligned_vector<float> c(n, 7);
    auto start = omp_get_wtime();
    for (int i = 0; i < iterations; ++i) {
      fma_and_swap_intrinsics(a.data(), b.data(), c.data(), di.data(), n);
    };
    cout << "fma and swap intrinsics: " << omp_get_wtime() - start << " seconds"
         << endl;
  }

  if (d != di || d != dt) {
    cout << "failed!!!\n";
  }
}
