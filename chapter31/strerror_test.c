/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             strerror_test.c
*
* Author:           garyparrot
* Created:          2019/08/02
* Description:      Thread-safe strerror()
******************************************************************************/

#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "tlpi_hdr.h"

static pthread_once_t once = PTHREAD_ONCE_INIT;

/* Thread key is maintain by a global(shared between all thread) array */
/* It will cost some space , SUSv3 require each Process have to support at least 128 thread keys */
static pthread_key_t strerrorKey;

#define MAX_ERROR_LEN 256

/* The destructor for Thread-specific data */
static void destructor(void *buf){ free(buf); }

static void createKey(void){
    int s;

    /* create the key for */
    if((s = pthread_key_create(&strerrorKey, destructor)))
        errExitEN(s, "pthread_key_create");
}

char *strerror(int err){

    /* This pthread_once guarantee createKey will be call at most once during runtime */
    /* You can do this at main for sure, but if this is a function in library, that't is infesible */
    int s = pthread_once(&once, createKey);
    if(s != 0) errExitEN(s, "pthread_once");

    /* Retrieve thread specific data from key */
    char *buf = pthread_getspecific(strerrorKey);
    /* Initialize it if NULL */
    if(buf == NULL){
        buf = malloc(MAX_ERROR_LEN);
        if(buf == NULL) errExit("malloc");
        /* Update specific data */
        if((s = pthread_setspecific(strerrorKey, buf)))
            errExitEN(s, "pthread_setspecific");
    }

    /* Real strerror() works */
    if(err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL)
        snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
    else{
        strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
        buf[MAX_ERROR_LEN - 1] = '\0';
    }

    return buf;
}

static void *threadFunc(void *arg){
    char *str;

    printf("Other thread about to call strerror()\n");
    str = strerror(EPERM);
    printf("Other thread: str (%p) = %s\n", str, str);

    return NULL;
}

int main(int argc, const char *argv[]){

    pthread_t t;
    int s;
    char *str;

    str = strerror(EINVAL);
    printf("Main thread has called strerror()\n");

    if((s = pthread_create(&t, NULL, threadFunc, NULL)))
        errExitEN(s, "pthread_create");

    if((s = pthread_join(t, NULL)))
        errExitEN(s, "pthread_join");

    printf("Main thread: str (%p) = %s\n", str, str);

    exit(EXIT_SUCCESS);
}
