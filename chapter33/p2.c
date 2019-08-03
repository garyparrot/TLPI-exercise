/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/08/03
* Description:      Exercise 2
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

static int show_pending;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mtx2= PTHREAD_MUTEX_INITIALIZER;

void *thread(void *arg){
    // fork
    long thread_id = (long)arg;
    if(thread_id == 0) {
        switch(fork()){
            case -1: errExit("fork");
            case  0:
                
                _exit(EXIT_SUCCESS);
        }
    }

    pthread_mutex_lock(&mtx2);
    while(show_pending == 0) pthread_cond_wait(&cond, &mtx);
    pthread_mutex_unlock(&mtx2);

    pthread_mutex_lock(&mtx);

    sigset_t sigset;
    sigpending(&sigset);

    printf("[THREAD%ld] Thread Pending signal: ", thread_id);
    for(int i = 1;i < NSIG; i++)
        if(sigismember(&sigset, i))
            printf("%d ", i);
    printf("\n");
    
    pthread_mutex_unlock(&mtx);

    return NULL;
}


int main(int argc, const char *argv[]){
    int s;
    pthread_t t0,t1,t2,t3,t4,t5;
    pthread_mutex_lock(&mtx2);

    sigset_t sigset;
    sigfillset(&sigset);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    if((s = pthread_create(&t0, NULL, thread, (void*)0 ))) errExitEN(s,"pthread_create");
    if((s = pthread_create(&t1, NULL, thread, (void*)1 ))) errExitEN(s,"pthread_create");
    if((s = pthread_create(&t2, NULL, thread, (void*)2 ))) errExitEN(s,"pthread_create");
    if((s = pthread_create(&t3, NULL, thread, (void*)3 ))) errExitEN(s,"pthread_create");
    if((s = pthread_create(&t4, NULL, thread, (void*)4 ))) errExitEN(s,"pthread_create");
    if((s = pthread_create(&t5, NULL, thread, (void*)5 ))) errExitEN(s,"pthread_create");

    show_pending = 1; pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mtx2);
    
    if((s = pthread_join(t0, NULL))) errExitEN(s, "pthread_join");
    if((s = pthread_join(t1, NULL))) errExitEN(s, "pthread_join");
    if((s = pthread_join(t2, NULL))) errExitEN(s, "pthread_join");
    if((s = pthread_join(t3, NULL))) errExitEN(s, "pthread_join");
    if((s = pthread_join(t4, NULL))) errExitEN(s, "pthread_join");
    if((s = pthread_join(t5, NULL))) errExitEN(s, "pthread_join");

    sigpending(&sigset);

    printf("[ MAIN ] Thread Pending signal: ");
    for(int i = 1;i < NSIG; i++)
        if(sigismember(&sigset, i))
            printf("%d ", i);
    printf("\n");

    return 0;
}
