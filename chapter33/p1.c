/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/08/03
* Description:      Exercise 1
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <signal.h>
#include "tlpi_hdr.h"

static int show_pending = 0;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *func(void *arg){

    while(show_pending == 0)
        pthread_cond_wait(&cond, &mtx);
    
    sigset_t sigset;
    if(sigpending(&sigset) == -1)
        errExit("sigpending");

    printf("[THREAD%ld] Pending signal: ", (long)arg);
    for(int i = 1; i < NSIG ; i++)
        if(sigismember(&sigset, i))
            printf("%d ", i);
    printf("\n");

    return NULL;
}

int main(int argc, const char *argv[]){

    pthread_t t1,t2;
    int s;

    sigset_t sigset;
    sigfillset(&sigset);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    if((s = pthread_create(&t1, NULL, func, (void*)1)))
        errExitEN(s, "pthread_create");
    if((s = pthread_create(&t2, NULL, func, (void*)2)))
        errExitEN(s, "pthread_create");

    // Sending Signal
    pthread_kill(t1, SIGUSR1);
    pthread_kill(t2, SIGUSR2);

    // broadcast cond
    show_pending = 1;
    pthread_cond_broadcast(&cond);

    if((s = pthread_join(t1, NULL)))
        errExitEN(s, "pthread_join");
    if((s = pthread_join(t2, NULL)))
        errExitEN(s, "pthread_join");
    
    return 0;
}
