#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define POOL_SIZE 3

struct work {
  void *(*function)(void);
  struct work *next;
};

typedef struct work work_t;

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
    printf("%s\n", "oh man");
    pthread_join(tp->threadQueue[i], NULL);
  }
  return EXIT_SUCCESS;
}