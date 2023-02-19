#include <iostream>

int main() {
  int* dummy = new int[2];
#pragma omp parallel num_threads(16)
  {
    // use Intel Inspector to detect race condition
    // fsanitize=undefined detects integer overflow
    dummy[0] += rand();
  }
  // fsanitize=address detects out-of-bounds access
  std::cout << dummy[2];
  // fsanitize=address detects memory leak
}
