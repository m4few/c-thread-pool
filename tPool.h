#pragma once
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

struct work {
  void *(*function)(void);
  struct work *next;
};

typedef struct work work_t;

struct threadPool {
  work_t *workQueuePtr;
  pthread_mutex_t workMutex;
  int threadCount;
  pthread_t *threadQueue;
};

typedef struct threadPool threadPool;

void *threadPoolRoutine(void *arg);
int threadPoolCreate(threadPool *tp, int n);
int threadPoolRun(threadPool *tp);
int threadPoolEnqueue(threadPool *tp, work_t *w);
