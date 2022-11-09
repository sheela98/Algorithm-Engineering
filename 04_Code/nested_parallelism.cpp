#include <iostream>
#include <omp.h>
using namespace std;

int main() {
  omp_set_nested(1); // enable nested parallelism
  int counter = 0; // counter for total threads executed
#pragma omp parallel num_threads(4) // start outer parallel region
  {
    // each thread creates two new threads
#pragma omp parallel num_threads(2) // start inner parallel region
    {
#pragma omp critical
      { // thread id in inner parallel region has nothing
        // to do with the thread id of the outer parallel region
        cout << omp_get_thread_num() << endl; // prints 0 or 1
        ++counter;
      }
    } // end inner parallel region
  } // end outer parallel region
  cout << "counter: " << counter << endl; // prints 8 since 4 * 2 = 8
}
