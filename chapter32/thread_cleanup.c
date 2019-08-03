/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             thread_cleanup.c
*
* Author:           garyparrot
* Created:          2019/08/03
* Description:      Demonstrate thread cleanup handler
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "tlpi_hdr.h"

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int glob = 0;

static void cleanupHandler(void *arg){
    printf("cleanup: freeing block at %p\n", arg );
    free(arg);
    printf("cleanup: unlocking mutex\n");
    int s;
    if((s = pthread_mutex_unlock(&mtx)))
        errExitEN(s, "pthread_mutex_unlock");
}

static void*  threadFunc(void *arg){
    int s;
    void *buf = NULL;

    buf = malloc(0x10000);
    printf("thread:  allocated memory at %p\n", buf);

    /* This can be a marco like {...} */
    /* Most of implementation make it a curly bracket, so better put push and pop at same level of code */
    pthread_cleanup_push(cleanupHandler, buf);
    
    while(glob == 0){
        if((s = pthread_cond_wait(&cond, &mtx)))
            errExitEN(s, "pthread_cond_wait");
    }

    /* cleanup with non-zero argument means cleanupHandler will get trigger when pop out */
    /* Provide cleaner code */
    printf("thread:  condition wait loop completed\n");
    pthread_cleanup_pop(1);

    /* Thread exit by return won't call cleanup handler */
    /* Thread exit by pthread_exit() will call cleanup handler */
    return NULL;
}  

int main(int argc, const char *argv[]){
    
    pthread_t thr;
    void *res;
    int s;

    if((s = pthread_create(&thr, NULL, threadFunc, NULL)))
        errExitEN(s, "pthread_create");

    sleep(2);

    if(argc == 1){
        printf("main:    about to cancel thread\n");
        if((s = pthread_cancel(thr)))
            errExitEN(s, "pthread_cancel");
    }else{
        printf("main:    about to signal condition variable\n");
        glob = 1;
        if((s = pthread_cond_signal(&cond)))
            errExitEN(s, "pthread_cond_signal");
    }

    if((s = pthread_join(thr, &res)))
        errExitEN(s, "pthread_join");

    if(res == PTHREAD_CANCELED)
        printf("main:    thread was canceled\n");
    else
        printf("main:    thread terminated normally\n");

    return 0;
}
