#include <iostream>

using namespace std;

// There are two tasks. One prints the word "race ".
// The other one prints "car ". The order in which tasks are
// executed is runtime dependent and may vary across multiple runs.
int main() {
#pragma omp parallel
  { // we need the single construct to guarantee that
    // each task is generated only once
#pragma omp single // one thread generates the tasks
    {
      // Each #pragma omp task directive defines a task
#pragma omp task // define task and put it in the task queue
      { cout << "race "; } // task 1
#pragma omp task // define task and put it in the task queue
      { cout << "car "; } // task 2
    } // end of single region (implicit barrier)
  } // end of parallel region
}
