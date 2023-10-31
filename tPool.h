#pragma once
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

struct threadPool {
  work_t *workQueuePtr;
  pthread_mutex_t workMutex;
  pthread_t threadQueue[POOL_SIZE];
};

typedef struct threadPool threadPool;

void *threadPoolRoutine(void *arg);
int threadPoolCreate(threadPool *tp);
int threadPoolRun(threadPool *tp);
int threadPoolEnqueue(threadPool *tp, work_t *w);
