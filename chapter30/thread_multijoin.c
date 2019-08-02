/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             thread_multijoin.c
*
* Author:           garyparrot
* Created:          2019/08/02
* Description:      Demonstrating how to join arbitrary thread.
******************************************************************************/

/* Condition variable 和 Mutex 一樣有提供靜態初始化 marco PTHREAD_COND_INITIALIZER */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "tlpi_hdr.h"

/* 靜態初始化Condition Variable */
static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
/* 靜態初始化 default mutex */
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

static int totThreads = 0;
static int numLive = 0;

static int numUnjoined = 0;

enum tstate{ TS_ALIVE, TS_TERMINATED, TS_JOINED };

static struct {
    pthread_t tid;
    enum tstate state;
    int sleepTime;
} *thread;

static void *threadFunc(void *arg){
    int idx = (int) arg;
    int s;

    /* Doing something */
    sleep(thread[idx].sleepTime);
    printf("Thread %d terminating\n", idx);

    /* acquire Mutex lock before access condition variable */
    if((s = pthread_mutex_lock(&threadMutex)))
        errExitEN(s, "pthread_mutex_lock");

    numUnjoined++;
    thread[idx].state = TS_TERMINATED;

    if((s = pthread_mutex_unlock(&threadMutex)))
        errExitEN(s, "pthread_mutex_unlock");
    if((s = pthread_cond_signal(&threadDied)))
        errExitEN(s, "pthread_cond_signal");

    return NULL;
}

int main(int argc, const char *argv[]){
    int s, idx;

    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s nsecs...\n", argv[0]);

    thread = calloc(argc - 1, sizeof(*thread));
    if(thread == NULL) errExit("calloc");

    for(idx = 0; idx < argc - 1; idx++){
        thread[idx].sleepTime = get_int(argv[idx+1], GN_NOFLAG, GN_ANY_BASE);
        thread[idx].state = TS_ALIVE;
        if((s = pthread_create(&thread[idx].tid, NULL, threadFunc, (void*)idx)))
            errExitEN(s, "pthread_create");
    }

    totThreads = argc - 1;
    numLive = totThreads;

    while(numLive > 0){
        /* Acquire mutex lock before access condition variable */
        if((s = pthread_mutex_lock(&threadMutex)))
            errExitEN(s, "pthread_mutex_lock");

        /* Condition Variable 返回時非代表條件已經發生，或許是虛假喚醒 */
        /* 把 Condition Variable wait返回解釋成事件 可能 發生比較好 */
        /* 所以不要用if,用while包住比較好 */
        while(numUnjoined == 0){
            if((s = pthread_cond_wait(&threadDied, &threadMutex)))
                errExitEN(s, "pthread_cond_wait");
        }

        /* Enumerate each thread struct to find which thread terminated */
        /* With this approach, we can wait for arbitrary thread */
        for(idx = 0; idx < totThreads; idx++){
            if(thread[idx].state == TS_TERMINATED){
                if((s = pthread_join(thread[idx].tid, NULL)))
                    errExitEN(s, "pthread_join");

                thread[idx].state = TS_JOINED;

                numLive--;
                numUnjoined--;

                printf("Reaped thread %d (numLive=%d)\n", idx, numLive);
            }
        }

        if((s = pthread_mutex_unlock(&threadMutex)))
            errExitEN(s, "pthread_mutex_unlock");
    }
    
    exit(EXIT_SUCCESS);
}
