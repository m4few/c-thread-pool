#include "tPool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *w() {
  printf("%s\n", "start");
  sleep(3);
  printf("%s\n", "stop");
  return EXIT_SUCCESS;
}

void *run() {

  while (1) {
    char buff[16];
    fgets(buff, 16, stdin);
    printf("%s", buff);
  }

  return NULL;
}

int main() {

  pthread_t mainThread;
  pthread_create(&mainThread, NULL, run, NULL);

  threadPool tp;
  threadPoolCreate(&tp, 4);
  work_t work = {w, NULL};
  work_t work2 = {w, NULL};
  work_t work3 = {w, NULL};
  work_t work4 = {w, NULL};
  threadPoolEnqueue(&tp, &work);
  threadPoolEnqueue(&tp, &work2);
  threadPoolEnqueue(&tp, &work3);
  threadPoolEnqueue(&tp, &work4);
  threadPoolRun(&tp);

  return EXIT_SUCCESS;
}
