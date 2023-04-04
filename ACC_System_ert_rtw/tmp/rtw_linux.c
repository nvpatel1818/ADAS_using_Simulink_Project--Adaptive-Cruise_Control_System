/* Copyright 2011-2019 The MathWorks, Inc. */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "rtw_linux.h"

#ifndef __USE_UNIX98
#define __USE_UNIX98
#endif

extern int pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
extern int pthread_mutexattr_settype(pthread_mutexattr_t *, int);

void rtw_pthread_mutex_init( void** mutexDW )
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_setprotocol(&attr, 1); /* PTHREAD_PRIO_INHERIT */
    #if defined(VXWORKS) || defined(__QNX__)
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    #else
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
    #endif
    *mutexDW = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init((pthread_mutex_t *)(*mutexDW), &attr);
}

typedef struct {
    void      (*func)(void);
    sem_t     semaphore;
    sem_t     doneSema;
    pthread_t thread;
    int       isPrioritySet;
} pthread_task_T;

void* rtw_worker_task(void *arg_) {
    pthread_task_T *arg = arg_;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    while (1) {
        sem_wait(&(arg->semaphore));
        arg->func(); /* execute the task body */
        sem_post(&(arg->doneSema));
    }
}

void* rtw_register_task(void(*f)(void)){
  pthread_task_T *arg = (pthread_task_T*)malloc(sizeof(pthread_task_T));
  sem_init(&(arg->semaphore), 0, 0);
  sem_init(&(arg->doneSema), 0, 0);
  arg->func = f;
  arg->isPrioritySet = 0;
  pthread_create(&(arg->thread), NULL, rtw_worker_task, arg);
  return arg;
}

void rtw_trigger_task(void *arg_){
  pthread_task_T *arg= (pthread_task_T*) arg_;
  int schedPolicy;
  struct sched_param schedParam;
  /* Adjust thread priority if different than the calling thread's priority */
  if (arg->isPrioritySet == 0) {
    pthread_getschedparam(pthread_self(), &schedPolicy, &schedParam);
    pthread_setschedparam(arg->thread, schedPolicy, &schedParam);
    arg->isPrioritySet = 1;
  }
  sem_post(&arg->semaphore);
}

void rtw_waitfor_task(void *arg){
  sem_wait(&(((pthread_task_T *)arg)->doneSema));
}

void rtw_deregister_task(void *arg_){
  int s;
  pthread_task_T *arg = arg_;
  pthread_cancel(arg->thread);
  s = pthread_join(arg->thread, NULL);
  if( s != 0){
    fprintf(stderr, "pthread_join returned error %d\n", s);
  }
  sem_destroy(&(arg->semaphore));
  sem_destroy(&(arg->doneSema));
  free(arg);
}
/* LocalWords:  PTHREAD PRIO pthread
 */
