/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             thread_incr.c
*
* Author:           garyparrot
* Created:          2019/08/01
* Description:      Mutex lock
******************************************************************************/

/* There are two variants of pthread_mutex_lock() */
/* pthread_mutex_trylock()   : try to acquire the lock. if failed, report EBUSY error */
/* pthread_mutex_timedlock() : try to acquire the lock within abstime(precision up to nanosecond if kernel supported)  */
/* Every time we acquire or release a lock, system call get executed. system call cost a few calculation but if the quantity is large it get slow */
/* Mutex perform machine language level atomic operation */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "tlpi_hdr.h"

/* pthread_mutex_destroy() initialize a dynamic pthread mutex */
/*      mutex init by dynamic way require pthread_mutex_destroy() to dispose it */
/* PTHREAD_MUTEX_INITIALIZER initialize a static pthread mutex with default attributes */
/*      mutex init by static way does need to dispose. */
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int glob = 0;

/* If lock owner aquire the same lock twice, it might cause a error or deadlock situation */
static void *threadFunc(void *arg){
    int loops = *((int*) arg), s;
    for(int i = 0;i < loops; i++){

        /* Try to acquire the lock */
        if((s = pthread_mutex_lock(&mtx)))
            errExitEN(s, "pthread_mutex_lock");

        int loc = glob;
        loc++;
        glob = loc;

        /* release the lock */
        if((s = pthread_mutex_unlock(&mtx)))
            errExitEN(s, "pthread_mutex_lock");
    }
    return NULL;
}

int main(int argc, const char *argv[]){

    pthread_t t1, t2;

    int loops = (argc > 1) ? get_int(argv[1], GN_NOFLAG, GN_ANY_BASE) : 10000000;
    int s;

    if((s = pthread_create(&t1, NULL, threadFunc, &loops)))
        errExitEN(s, "pthread_create");

    if((s = pthread_create(&t2, NULL, threadFunc, &loops)))
        errExitEN(s, "pthread_create");

    if((s = pthread_join(t1, NULL)))
        errExitEN(s, "pthread_join");

    if((s = pthread_join(t2, NULL)))
        errExitEN(s, "pthread_join");

    printf("Glob = %d\n", glob);
    
    return 0;
}
