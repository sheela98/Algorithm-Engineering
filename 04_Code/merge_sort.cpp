#include <algorithm>
#include <cstring>
#include <iostream>
#include <omp.h>
#include <random>
using namespace std;

vector<int> get_random_int_vector(int n) {
  default_random_engine re{random_device{}()};
  uniform_int_distribution<int> next_rand{INT32_MIN, INT32_MAX};
  vector<int> v(n);
  for (auto &num : v) {
    num = next_rand(re);
  }
  return v;
}

// https://www.geeksforgeeks.org/insertion-sort/
void insertion_sort(int *arr, int n) {
  int i, key, j;
  for (i = 1; i < n; i++) {
    key = arr[i];
    j = i - 1;

    /* move elements of arr[0..i-1], that are
    greater than key, to one position ahead
    of their current position */
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

inline void merge(const int *__restrict__ a, const int *__restrict__ b,
                  int *__restrict__ c, const int a_size, const int b_size,
                  const int c_size) {
  int idx_a = 0;
  int idx_b = 0;
  for (int i = 0; i < c_size; ++i) {
    if (idx_a == a_size) {
      c[i] = b[idx_b++];
    } else if (idx_b == b_size) {
      c[i] = a[idx_a++];
    } else {
      c[i] = (a[idx_a] < b[idx_b]) ? a[idx_a++] : b[idx_b++];
    }
  }
}

void merge_sort(int *arr, int n) {
  if (n > 1) {
    if (n < 32) { // insertion sort for n smaller than 32
      insertion_sort(arr, n);
      return;
    }
    const int size_a = n / 2;
    const int size_b = n - size_a;
#pragma omp task if (size_a > 10000) // make task if size_a > 10000, one task is enough
    merge_sort(arr, size_a); // parallel recursive call
    merge_sort(arr + size_a, size_b); // recursive call
#pragma omp taskwait // wait until both subarrays are sorted
    if (n < 8192) { // allocate array on the stack for small n
      // sometimes called _alloca() or _malloca(), or "int c[n]"
      int* c = (int*) alloca(n * sizeof(int));
      merge(arr, arr + size_a, c, size_a, size_b, n);
      memcpy(arr, c, sizeof(int) * n); // copying can be tuned away...
      return;
    }
    int *c = new int[n];
    merge(arr, arr + size_a, c, size_a, size_b, n);
    memcpy(arr, c, sizeof(int) * n); // copying can be tuned away...
    delete[](c);
  }
}

void merge_sort_run(int *arr, int n) {
#pragma omp parallel
#pragma omp single nowait
  merge_sort(arr, n);
}

int main(int argc, char *argv[]) {
  const int n = 100000000;
  vector<int> v = get_random_int_vector(n);
  vector<int> v_copy = v;

  double start = omp_get_wtime();
  merge_sort_run(v.data(), n);
  cout << "naive: " << omp_get_wtime() - start << " seconds" << endl;

  start = omp_get_wtime();
  sort(begin(v_copy), end(v_copy));
  cout << "std::sort: " << omp_get_wtime() - start << " seconds" << endl;

  if (v != v_copy) {
    cout << "sort implementation is buggy\n";
  }
}
