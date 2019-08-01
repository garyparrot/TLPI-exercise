/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             detached_attrib.c
*
* Author:           garyparrot
* Created:          2019/08/01
* Description:      Create thread with attributes.
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

void *func(void *arg){ return NULL; }

int main(int argc, const char *argv[]){

    pthread_t t1;
    pthread_attr_t attr;
    int s;

    if((s = pthread_attr_init(&attr))) 
        errExitEN(s, "pthread_attr_init");

    if((s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)))
        errExitEN(s, "pthread_attr_setdetachstate");

    /* Create thread with attributes, which detach as default */
    if((s = pthread_create(&t1, &attr, func, (void*)1)))
        errExitEN(s, "pthread_create");

    if((s = pthread_attr_destroy(&attr)))
        errExitEN(s, "pthread_attr_destroy");

    return 0;
}
