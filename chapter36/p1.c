/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/09/11
* Description:      exercise 1
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>

#include "tlpi_hdr.h"

double sumtime2sec(struct timeval a, struct timeval b){
    return ((double)(a.tv_sec + b.tv_sec)*1e6 + (a.tv_usec + b.tv_usec)) / 1e6;
}

int main(int argc, const char *argv[]){

    struct rusage res;
    sigset_t sig;
    pid_t child;

    sigemptyset(&sig);
    sigaddset(&sig, SIGUSR1);

    for(int i = 0;i < 1e9; i++);
    sigprocmask(SIG_BLOCK, &sig, NULL);

    switch(child = fork()){
        case -1: errExit("fork");
        case  0: 
            /* uselessssss job */
            for(int i = 0;i < 1e9; i++);

            /* wait for signal from parent */
            sigemptyset(&sig);
            sigaddset(&sig, SIGUSR1);
            sigwait(&sig, NULL);
            break;
        default:

            getrusage(RUSAGE_CHILDREN, &res);
            printf("CPU: %lf\n", sumtime2sec(res.ru_utime, res.ru_stime));

            /* send it */
            kill(child, SIGUSR1);

            /* wait for child end its job */
            wait(NULL);

            getrusage(RUSAGE_CHILDREN, &res);
            printf("CPU: %lf\n", sumtime2sec(res.ru_utime, res.ru_stime));
    }
    
    return 0;
}
