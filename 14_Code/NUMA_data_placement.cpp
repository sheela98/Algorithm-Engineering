#include <fstream>
#include <iostream>

using namespace std;

// create random data (on linux)
// head -c 1GB /dev/urandom > 1GB_random_data

int main() {
  ifstream ifs("1GB_random_data", ios::binary | ios::ate);
  size_t size = ifs.tellg();
  ifs.seekg(0);

  auto* a = (int*)malloc(size);   // No calloc! No vector!
  auto* b = (int*)malloc(size);   // No calloc! No vector!
  size_t n = size / sizeof(int);  // number of integers

  // redundant parallel initialization to place the data upfront
  // data is placed in the memories closest to the threads executing the loop
#pragma omp parallel for schedule(static)
  for (size_t i = 0; i < n; i++) {
    a[i] = 0;
  }

  // at this point, the placement has been decided upon already
  ifs.read((char*)a, size);  // read data to array a

  // each thread processes data from the memory closest to it
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n; i++) {
    b[i] = 2 * a[i];
  }

  free(a);
  free(b);
}
