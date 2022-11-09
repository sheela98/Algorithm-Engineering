#include <iostream>

using namespace std;

int main() {
#pragma omp parallel
  { // we need the single construct to guarantee that
    // each task is generated only once
#pragma omp single // one thread generates the tasks
    {
      cout << "A "; // printed before tasks are generated
      // Each #pragma omp task directive defines a task
#pragma omp task // define task and put it in the task queue
      { cout << "race "; } // task 1
#pragma omp task // define task and put it in the task queue
      { cout << "car "; } // task 2
#pragma omp taskwait // wait here until the tasks are completed
      cout << "is fun to watch.\n";
    } // end of single region
  } // end of parallel region
}
