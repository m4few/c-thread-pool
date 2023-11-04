#include "tPool.h"

void *threadPoolRoutine(void *arg) {
  threadPool *pool = (threadPool *)arg;

  // TODO: use pthread_cond_signal to avoid this
  while (1) {
    pthread_mutex_lock(&(pool->workMutex));
    pthread_cond_wait(&pool->cond, &pool->workMutex);

    if (pool->workQueuePtr == NULL) {
      pthread_mutex_unlock(&(pool->workMutex));
      continue;
    }

    work_t job = *(pool->workQueuePtr);
    pool->workQueuePtr = job.next;

    pthread_mutex_unlock(&(pool->workMutex));

    job.function(job.arg);

    pthread_cond_broadcast(&pool->cond);
  }
  return NULL;
}

int threadPoolCreate(threadPool *tp, int n) {
  pthread_t thread;
  tp->threadCount = n;
  tp->threadQueue = malloc(sizeof(pthread_t) * tp->threadCount);
  pthread_mutex_init(&tp->workMutex, NULL);
  pthread_cond_init(&tp->cond, NULL);

  for (int i = 0; i < tp->threadCount; i++) {
    tp->threadQueue[i] = thread;
    pthread_create(tp->threadQueue + i, NULL, threadPoolRoutine, tp);
  }
  return EXIT_SUCCESS;
}

int threadPoolRun(threadPool *tp) {
  for (int i = 0; i < tp->threadCount; i++) {
    pthread_join(tp->threadQueue[i], NULL);
  }
  return EXIT_SUCCESS;
}

int threadPoolEnqueue(threadPool *tp, work_t *w) {
  if (tp->workQueuePtr == NULL) {
    tp->workQueuePtr = w;
  } else {

    work_t *node = tp->workQueuePtr;
    while (node->next != NULL) {
      node = node->next;
    }

    node->next = w;
  }
  pthread_cond_broadcast(&tp->cond);
  return EXIT_SUCCESS;
}
