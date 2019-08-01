/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             simple_thread.c
*
* Author:           garyparrot
* Created:          2019/08/01
* Description:      Demonstrating Pthread functions.
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
    char *s = (char*) arg;

    printf("%s", s);

    return (void*) strlen(s);
}

int main(int argc, const char *argv[]){

    /* The handler for pthread API, note this is not Thread ID(TID) which maintained by kernel. */
    /* They are different stuff. */
    pthread_t t1;
    void *res;

    int s = pthread_create(&t1, NULL, threadFunc, "Hello World\n");
    /* pthread return errno by function result. */
    if(s != 0) errExitEN(s, "pthread_create");

    printf("Message from main()\n");

    /* Join the thread, wait for it exit. */
    /* Any thread can join any thread if one know its pthread handler */
    /* Every thread must be joined, otherwise will becomes a zombie thread. */
    /* Detach a thread mask one thread dispose automatically after exit without joining. */
    s = pthread_join(t1, &res);

    if(s != 0) errExitEN(s, "pthread_join");

    printf("Thread returned %ld\n", (long)res);

    exit(EXIT_SUCCESS);
}
