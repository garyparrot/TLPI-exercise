/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/08/01
* Description:      Exercise 1
******************************************************************************/

// Cause EDEADLK error.

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "tlpi_hdr.h"

void *thread_func(void *arg){
    int ret = 0;
    pthread_t tid = pthread_self();

    /* Prevent lock myself */
    /* Can't prevent two thread lock each other. */
    if(!pthread_equal(tid, pthread_self()))
        ret = pthread_join(tid, NULL);

    if(ret != 0)
        errExitEN(ret, "pthread_join");
    return NULL;
}

int main(int argc, const char *argv[]){

    pthread_t t1;

    int s = pthread_create(&t1, NULL, thread_func, NULL);
    if(s != 0) 
        errExit("pthread_create");

    pthread_join(t1, NULL);
    
    return 0;
}
