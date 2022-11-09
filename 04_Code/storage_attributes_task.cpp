#include <iostream>
using namespace std;

int main() {
  int x = 1; // x is shared in a task per default -> outside parallel region
#pragma omp parallel num_threads(4) // begin parallel region
  {
    int y = 7; // y is firstprivate in a task per default -> inside parallel region
#pragma omp single // the code in a single is executed only once
    {
      cout << "x: ";
      for (int i = 0; i < 8; ++i) { // create 8 tasks
#pragma omp task firstprivate(x) shared(y) // overwrite storage attributes
        {
          cout << ++x; // prints always 2 because x is firstprivate in each task
          ++y; // this y comes from the thread that creates the single
          // but y is updated from multiple threads -> possible race condition
        }
      }
    } // implicit barrier of single
#pragma omp critical
    cout << "\ny: " << y; // prints 7 7 7 and (very likely) one 15
  } // end parallel region
}
