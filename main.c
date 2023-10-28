#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define POOL_SIZE 3

typedef struct {
  void *function;
  struct work *next;
} work;

struct testWork {
  int id;
  struct testWork *next;
};

typedef struct testWork testWork_t;

typedef struct {
  struct testWork *workQueuePtr;
  pthread_mutex_t workMutex;
  pthread_t threadQueue[POOL_SIZE];
} threadPool;

void *threadPoolRoutine(void *arg) {
  threadPool *pool = (threadPool *)arg;

  while (1) {
    usleep(20);
    pthread_mutex_lock(&(pool->workMutex));

    if (pool->workQueuePtr != NULL) {
      testWork_t job = *(pool->workQueuePtr);
      pool->workQueuePtr = job.next;
      printf("thread:%d int:%d\n", (int)pthread_self(), job.id);
    } else {
      // printf("%s\n", "no jobs available");
    }

    pthread_mutex_unlock(&(pool->workMutex));
  }
  return NULL;
}

int threadPoolCreate(threadPool *tp) {
  pthread_t thread;
  for (int i = 0; i < POOL_SIZE; i++) {
    tp->threadQueue[i] = thread;
    pthread_create(tp->threadQueue + i, NULL, threadPoolRoutine, tp);
  }
  return EXIT_SUCCESS;
}

int threadPoolRun(threadPool *tp) {
  for (int i = 0; i < POOL_SIZE; i++) {
    pthread_join(tp->threadQueue[i], NULL);
  }
  return EXIT_SUCCESS;
}

int main() {
  threadPool testPool;

  testWork_t d;
  d.id = 4;
  d.next = NULL;

  testWork_t c;
  c.id = 3;
  c.next = &d;

  testWork_t b;
  b.id = 2;
  b.next = &c;

  testWork_t a;
  a.id = 1;
  a.next = &b;

  threadPoolCreate(&testPool);
  testPool.workQueuePtr = &a;

  threadPoolRun(&testPool);

  return EXIT_SUCCESS;
}
