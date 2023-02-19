#include "avx2sort.h"
#include <functional>
#include <random>
#include <vector>

#if defined(__INTEL_COMPILER)
#include <ipp.h>
#endif

/*
 * see https://github.com/viktorleis/perfevent
 * for installation and usage
 */
#include "PerfEvent.hpp"

using namespace std;

/* this code computes performance indicators for std::sort,
 * vectorized quicksort and IPP radix sort */

int main() {
  int n = 100000000;

  /* create a vector with random integers */
  vector<int> a(n);
  auto rand_int = bind(uniform_int_distribution<int>{INT32_MIN, INT32_MAX}, default_random_engine{std::random_device{}()});
  for (int i = 0; i < n; ++i) {
    a[i] = rand_int();
  }
  auto b = a;

  BenchmarkParameters params;
  params.setParam("n",to_string(n));

  /* std::sort */
  {
    params.setParam("_name"," std::sort");
    PerfEventBlock e(n, params, true);
    std::sort(begin(b), end(b));
  }

  /* vectorized quicksort */
  {
    auto c = a;
    {
      params.setParam("_name","avx2::sort");
      PerfEventBlock e(n, params, false);
      avx2::quicksort(c.data(), n);
    }
    if(c != b) puts("vectorized quicksort failed!\n");
  }

#if defined(__INTEL_COMPILER)
  /* IPP radix sort */
  {
    IppSizeL sz;
    Ipp8u *buffer;
    ippsSortRadixGetBufferSize_L(n, ipp32s, &sz);
    buffer = (Ipp8u*) malloc(sz);
    {
      params.setParam("_name"," IPP radix");
      PerfEventBlock e(n, params, false);
      ippsSortRadixAscend_32s_I_L(a.data(), n, buffer);
    }
    free(buffer);
    if(a != b) puts("vectorized quicksort failed!\n");
  }
#endif
}
