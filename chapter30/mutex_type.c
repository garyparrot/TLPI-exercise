/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             mutex_type.c
*
* Author:           garyparrot
* Created:          2019/08/02
* Description:      Initialize a lock with different type
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

int main(int argc, const char *argv[]){

    int s;
    pthread_mutex_t mtx;
    pthread_mutexattr_t mtxAttr;

    if((s = pthread_mutexattr_init(&mtxAttr)))
        errExitEN(s, "pthread_mutexattr_init");

    /* PTHREAD_MUTEX_NORMAL : Just normal mutex, no error check. allow thread lock its own lock. */
    /* PTHREAD_MUTEX_ERRORCHECK : Checking error for all the operation. It is slower than others. But useful for debug. */
    /* PTHREAD_MUTEX_RECURSIVE : Once somebody try to acquire the mutex, it increase a counter for mutex by 1. Once 
     *                           the value decrease to 0. The mutex get destroy automatically. */
    if((s = pthread_mutexattr_settype(&mtxAttr, PTHREAD_MUTEX_ERRORCHECK)))
        errExitEN(s, "pthread_mutexattr_settype");

    if((s = pthread_mutex_init(&mtx, &mtxAttr)))
        errExitEN(s, "pthread_mutex_init");

    /* destroy attributes */
    if((s = pthread_mutexattr_destroy(&mtxAttr)))
        errExitEN(s, "pthread_mutexattr_destroy");

    /* destroy mutex */
    if((s = pthread_mutex_destroy(&mtx)))
        errExitEN(s, "pthread_mutex_destroy");
    
    return 0;
}
