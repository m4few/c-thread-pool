#include "tPool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *w() {
  printf("%s\n", "start");
  sleep(3);
  printf("%s\n", "stop");
  return EXIT_SUCCESS;
}

int main() {
  threadPool tp;
  threadPoolCreate(&tp);
  work_t work = {w, NULL};
  work_t work2 = {w, NULL};
  // work_t work2 = {w, NULL};
  // threadPoolEnqueue(&tp, &work);
  // threadPoolEnqueue(&tp, &work2);
  threadPoolEnqueue(&tp, &work);
  threadPoolEnqueue(&tp, &work2);
  threadPoolRun(&tp);

  return EXIT_SUCCESS;
}
