/* Copyright 2011-2016 The MathWorks, Inc. */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include "rtw_linux_mac.h"

#ifndef __USE_UNIX98
#define __USE_UNIX98
#endif

extern int pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);

extern int pthread_mutexattr_settype(pthread_mutexattr_t *, int);

void rtw_pthread_mutex_init_mac( void** mutexDW )
{
    pthread_mutexattr_t attr;       
    pthread_mutexattr_setprotocol(&attr, 1);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    *(mutexDW) = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init((pthread_mutex_t *)(*(mutexDW)), &attr);
}

void rtw_pthread_sem_create_mac( void** semaphoreDW1, void** semaphoreDW2, long initVal )
{
    static int semcount = 0;
    char* semName = malloc(sizeof(char)*32);
    sprintf(semName, "sem_sync_%x%x", semcount++, getpid());
    
    *semaphoreDW1 = sem_open(semName, O_CREAT | O_EXCL, 0777, (int)initVal);
    *semaphoreDW2 = (void*)semName;
}

typedef struct {
    void      (*func)(void);
    sem_t*     semaphore;
    char*      semaphoreName;
    sem_t*     doneSemaphore;
    char*      doneSemaphoreName;
    pthread_t thread;
} pthread_task_T;

void* rtw_worker_task(void *_arg) {
    pthread_task_T *arg = _arg;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    while (1) {
        sem_wait(arg->semaphore);
        arg->func(); /* execute the task body */
        sem_post(arg->doneSemaphore);
    }
}

void* rtw_register_task(void(*f)(void)){
  static int semcount = 0;
  pthread_task_T *arg = (pthread_task_T*)malloc(sizeof(pthread_task_T));
  arg->semaphoreName = malloc(sizeof(char)*32);
  arg->doneSemaphoreName = malloc(sizeof(char)*32);
  sprintf(arg->semaphoreName, "Semaphore_%x", semcount);
  sprintf(arg->doneSemaphoreName, "doneSemaphore_%x", semcount++);
  arg->semaphore = sem_open(arg->semaphoreName, O_CREAT | O_EXCL, 0777, 0);
  arg->doneSemaphore = sem_open(arg->doneSemaphoreName, O_CREAT | O_EXCL, 0777, 0);
  arg->func = f;
  pthread_create(&(arg->thread), NULL, rtw_worker_task, arg);
  return arg;
}

void rtw_trigger_task(void *arg){
  sem_post(((pthread_task_T *)arg)->semaphore);
}

void rtw_waitfor_task(void *arg){
  sem_wait(((pthread_task_T *)arg)->doneSemaphore);
}

void rtw_deregister_task(void *_arg){
  int s;
  pthread_task_T *arg = _arg;
  pthread_cancel(arg->thread);
  s = pthread_join(arg->thread, NULL);
  if(s != 0){
    fprintf(stderr, "pthread_join returned error %d\n", s);
  }
  sem_close(arg->semaphore);
  sem_close(arg->doneSemaphore);
  sem_unlink(arg->semaphoreName);
  sem_unlink(arg->doneSemaphoreName);
  free(arg->semaphoreName);
  free(arg->doneSemaphoreName);
  free(arg);
}
/* LocalWords:  PTHREAD PRIO DW
 */
