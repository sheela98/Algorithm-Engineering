#include <iostream>
#include <omp.h>
using namespace std;

int main() {
  volatile int problem_size = 100;
  // if the problem size is small, don't execute in parallel
#pragma omp parallel if (problem_size > 1000)
  cout << "Hello from thread " << omp_get_thread_num() << endl;
}
