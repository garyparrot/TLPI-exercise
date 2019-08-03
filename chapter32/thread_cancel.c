/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             thread_cancel.c
*
* Author:           garyparrot
* Created:          2019/08/03
* Description:      Demonstrating how to cancel a thread
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

static void *threadFunc(void *arg){
    printf("New thread started\n");

    for(int i = 1; ; i++){
        printf("Loop %d\n",i);
        sleep(1);
    }

    /* Never ever get executed */
    return NULL;
}

int main(int argc, const char *argv[]){
    pthread_t thr;
    int s;
    void *res;

    // This create a new thread
    // Cancel is enable by default.
    // And cancel type is DEFERED, thread only terminated at cancellation point
    if((s = pthread_create(&thr, NULL, threadFunc, NULL)))
        errExitEN(s, "pthread_create");
    
    sleep(5);

    /* cancel a thread */
    if((s = pthread_cancel(thr)))
        errExitEN(s, "pthread_cancel");

    /* canceled thread have must be join */
    if((s = pthread_join(thr, &res)))
        errExitEN(s, "pthread_join");

    printf("%s\n", (res == PTHREAD_CANCELED) ? "Thread was canceled" : "what");
    
    return 0;
}
