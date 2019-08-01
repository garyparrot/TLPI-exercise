/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_pthread_detach.c
*
* Author:           garyparrot
* Created:          2019/08/01
* Description:      Demonstrate pthread_detach()
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

void *func(void* v){

    pthread_detach(pthread_self());
    printf("Detach myself.\n");

    return NULL;
}

int main(int argc, const char *argv[]){
    
    pthread_t t1;

    int s = pthread_create(&t1, NULL, func, NULL);
    if(s != 0) errExitEN(s, "pthread_create");

    // if the main thread exited, every thread will be stopped.
    sleep(1);

    return 0;
}
