#include <iostream>
#include <omp.h>
#include <vector>

void dummy(int increment) {
  std::vector<int> v(100 * increment);
  // repeatedly updating on 100 different cache lines one value
  for (int i = 0; i < 10000000; ++i) {
    for (int j = 0; j < v.size(); j+=increment) {
      ++v[j];
    }
  }
}

int main() {
  using namespace std;
  {
    auto start = omp_get_wtime();
    dummy(1020);
    cout << "increment 1020: " << omp_get_wtime() - start << " seconds" << endl;
  }
  {
    auto start = omp_get_wtime();
    dummy(1024); // increment leads to L1 cache conflicts
    cout << "increment 1024: " << omp_get_wtime() - start << " seconds" << endl;
  }
  {
    auto start = omp_get_wtime();
    dummy(1028);
    cout << "increment 1028: " << omp_get_wtime() - start << " seconds" << endl;
  }
}
