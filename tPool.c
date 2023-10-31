#include "tPool.h"
#include <stdlib.h>

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

int threadPoolEnqueue(threadPool *tp, work_t *w) {

  if (tp->workQueuePtr == NULL) {
    tp->workQueuePtr = w;
    return EXIT_SUCCESS;
  }

  work_t *node = tp->workQueuePtr;
  while (node->next != NULL) {
    node = node->next;
  }

  node->next = w;

  return EXIT_SUCCESS;
}
