#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define POOL_SIZE 2

struct work {
  void *(*function)(void);
  struct work *next;
};

typedef struct work work_t;

struct testWork {
  int id;
  struct testWork *next;
};

typedef struct testWork testWork_t;

typedef struct {
  work_t *workQueuePtr;
  pthread_mutex_t workMutex;
  pthread_t threadQueue[POOL_SIZE];
} threadPool;

void *threadPoolRoutine(void *arg) {
  threadPool *pool = (threadPool *)arg;

  // TODO: use pthread_cond_signal to avoid this
  while (1) {
    usleep(20);
    pthread_mutex_lock(&(pool->workMutex));

    if (pool->workQueuePtr == NULL) {
      pthread_mutex_unlock(
          &(pool->workMutex)); // TODO: this is also sort of bad but it would be
                               // fixed by a signal
      continue;
    }

    work_t job = *(pool->workQueuePtr);
    pool->workQueuePtr = job.next;

    pthread_mutex_unlock(&(pool->workMutex));

    job.function();
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

void *w() {
  printf("this is happening wooo\n");
  sleep(3);
  printf("wimwamwimwamwimwam\n");
  return NULL;
}

int main() {
  threadPool testPool;

  work_t d;
  d.function = w;
  d.next = NULL;

  work_t c;
  c.function = w;
  c.next = &d;

  work_t b;
  b.function = w;
  b.next = &c;

  work_t a;
  a.function = w;
  a.next = &b;

  printf("befooooore\n");
  threadPoolCreate(&testPool);
  testPool.workQueuePtr = &a;
  printf("2!\n");
  threadPoolRun(&testPool);
  printf("oh yeah execution is not blocked L>>>\n");

  return EXIT_SUCCESS;
}
