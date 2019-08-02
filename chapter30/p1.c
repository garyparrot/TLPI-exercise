/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             thread_incr.c
*
* Author:           garyparrot
* Created:          2019/08/01
* Description:      Show the importance of synchronization between threads
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

struct info{
    int thread_identifier;
    int loops;
};

static int glob = 0;

static void *threadFunc(void *arg){
    struct info *info = ((struct info*) arg);
    for(int i = 0;i < info->loops; i++){
        /* In fact you can change this line to glob++ */
        /* But actually, at some platform(e.g. RISC) glob++ will be translate into machine code like below */
        /* So there is no way you can guarantee this code is atomic */
        int loc = glob; loc++; glob = loc;
        printf("[ %2d ] Current value : %d\n", info->thread_identifier, glob);
    }
    return NULL;
}

int main(int argc, const char *argv[]){

    pthread_t t1, t2;

    int loops = (argc > 1) ? get_int(argv[1], GN_NOFLAG, GN_ANY_BASE) : 10000000;
    int s;

    struct info ti1 = { .thread_identifier = 1, .loops = loops };
    struct info ti2 = { .thread_identifier = 2, .loops = loops };

    if((s = pthread_create(&t1, NULL, threadFunc, &ti1)))
        errExitEN(s, "pthread_create");

    if((s = pthread_create(&t2, NULL, threadFunc, &ti2)))
        errExitEN(s, "pthread_create");

    if((s = pthread_join(t1, NULL)))
        errExitEN(s, "pthread_join");

    if((s = pthread_join(t2, NULL)))
        errExitEN(s, "pthread_join");

    printf("Glob = %d\n", glob);
    
    return 0;
}
