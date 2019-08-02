/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             strerror_test.c
*
* Author:           garyparrot
* Created:          2019/08/02
* Description:      Thread-safe strerror() with Thread-local storage
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

#define MAX_ERROR_LEN 256

/* Thread-local storage */
/* Each thread has their thread-local variable copy */
/* This variable will stay with each thread until terminated */
/* In order to use this, require  */
/* 1. Kernel support(Linux 2.6 up) */
/* 2. Implementation support(POSIX thread) */
/* 3. Compiler support(at least gcc 3.3 on x86 platform ) */
static __thread char buf[MAX_ERROR_LEN];

char *strerror(int err){

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
