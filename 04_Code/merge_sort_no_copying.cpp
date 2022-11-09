#include <algorithm>
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

void merge_sort(int *arr, int *buffer, int n); // forward declaration of merge_sort

void merge_sort_helper(int *arr, int *buffer, int n) { // helps to implement no copying strategy
  const int size_a = n / 2;
  const int size_b = n - size_a;
#pragma omp task final(size_a < 10000) // stop creating tasks if size_a < 10000
  merge_sort(arr, buffer, size_a); // parallel recursive call to merge sort
  merge_sort(arr + size_a, buffer + size_a, size_b); // recursive call to merge sort
#pragma omp taskwait // wait until both subarrays are sorted
  merge(arr, arr + size_a, buffer, size_a, size_b, n); // merge from arr into buffer
}

void merge_sort(int *arr, int *buffer, int n) {
  if (n > 1) { // only this function can end a recursion
    if (n < 64) { // insertion sort for n smaller than 64
      insertion_sort(arr, n);
      return;
    }
    const int size_a = n / 2;
    const int size_b = n - size_a;
#pragma omp task final(size_a < 10000) // stop creating tasks if size_a < 10000
    merge_sort_helper(arr, buffer, size_a); // parallel recursive call to helper
    merge_sort_helper(arr + size_a, buffer + size_a, size_b); // recursive call to helper
#pragma omp taskwait // wait until both subarrays are sorted
    merge(buffer, buffer + size_a, arr, size_a, size_b, n); // merge from buffer into arr
  }
}

void merge_sort_run(int *arr, int *buffer, int n) { // buffer is O(n) extra memory
#pragma omp parallel								// user provides buffer when calling merge_sort_run
#pragma omp single nowait
  merge_sort(arr, buffer, n); 
}

int main(int argc, char *argv[]) {
  const int n = 100000000;
  vector<int> v = get_random_int_vector(n);
  vector<int> v_copy = v;
  vector<int> buffer(n); // extra memory for merge sort

  double start = omp_get_wtime();
  merge_sort_run(v.data(), buffer.data(), n);
  cout << "with buffer: " << omp_get_wtime() - start << " seconds" << endl;

  start = omp_get_wtime();
  sort(begin(v_copy), end(v_copy));
  cout << "std::sort: " << omp_get_wtime() - start << " seconds" << endl;

  if (v != v_copy) {
    cout << "sort implementation is buggy\n";
  }
}
