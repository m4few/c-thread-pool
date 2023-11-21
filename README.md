# c-thread-pool
A simple implementation of a thread pool in C.

# usage
```
// include the library
#include "tPool.h"

// example function
int example(int i) {
  return i + 1;
}

int main() {
  // make a new thread pool
  threadPool tp = THREAD_POOL_INIT;

  int numberOfWorkers = 3;
  threadPoolCreate(&tp, numberOfWorkers);

  // give the thread pool a job
  int argument = 2;
  work_t w1 = {example, argument, NULL};
  threadPoolEnqueue(&tp, &w1);

  // start the pool
  threadPoolRun(&tp);

}
