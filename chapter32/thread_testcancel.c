/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             thread_cancel.c
*
* Author:           garyparrot
* Created:          2019/08/03
* Description:      Demonstrating testcancel
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

/* If a function has no cancellation point, the cancel request will keep suspended */
/* pthread_testcancel() can test if the cancel request is suspended. */
/* if so, thread terminated immediately */
static void *threadFunc(void *arg){
    printf("New thread started\n");

    int value = 0;

    for(int i = 1; i < 1000000000 ; i++){ value += 1; }

    printf("%d\n", value);
    pthread_testcancel();
    printf("%d\n", value);

    for(int i = 1; i <= 1000000000 ; i++){ value += 1; }

    return NULL;
}

int main(int argc, const char *argv[]){
    pthread_t thr;
    int s;
    void *res;

    if((s = pthread_create(&thr, NULL, threadFunc, NULL)))
        errExitEN(s, "pthread_create");
    
    if((s = pthread_cancel(thr)))
        errExitEN(s, "pthread_cancel");

    if((s = pthread_join(thr, &res)))
        errExitEN(s, "pthread_join");

    printf("%s\n", (res == PTHREAD_CANCELED) ? "Thread was canceled" : "what");
    
    return 0;
}
