#include "avx2sort.h"
#include <functional>
#include <random>
#include <vector>

/*
 * see https://github.com/viktorleis/perfevent
 * for installation and usage
 */
#include "PerfEvent.hpp"

using namespace std;

/* this code computes performance indicators for std::nth_element and vectorized
 * quickselect */

int main() {
  int n = 100000000;
  int k = n / 2;

  /* create a vector with random integers */
  vector<int> a(n);
  auto rand_int = bind(uniform_int_distribution<int>{INT32_MIN, INT32_MAX},
                       default_random_engine{std::random_device{}()});
  for (int i = 0; i < n; ++i) {
    a[i] = rand_int();
  }
  auto b = a;

  BenchmarkParameters params;
  params.setParam("n", to_string(n));

  /* std::nth_element */
  {
    params.setParam("_name", " std::nth_element");
    PerfEventBlock e(n, params, true);
    std::nth_element(begin(a), begin(a) + k, end(a));
  }

  /* vectorized quickselect */
  {
    {
      params.setParam("_name", "avx2::quickselect");
      PerfEventBlock e(n, params, false);
      avx2::quickselect(b.data(), n, k);
    }
  }

  avx2::quicksort(a.data(), k);
  avx2::quicksort(b.data(), k);
  avx2::quicksort(a.data() + k + 1, n - k - 1);
  avx2::quicksort(b.data() + k + 1, n - k - 1);
  if(a != b || a[k] != b[k])
    cerr << "failed!";
}
