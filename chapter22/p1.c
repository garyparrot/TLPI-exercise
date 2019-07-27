/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/27
* Description:      Chapter 22 Exercise 1
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "tlpi_hdr.h"

void handler(int sig){
    printf("Hi");
}

int main(int argc, const char *argv[]){
    
    sigset_t empty;
    sigemptyset(&empty);
    sigaddset(&empty, SIGCONT);
    if(sigprocmask(SIG_BLOCK, &empty, NULL) == -1)
        errExit("sigprocmask");

    printf("SIGCONT blocked.\n");

    struct sigaction siga;
    siga.sa_handler = handler;
    siga.sa_flags = 0;
    sigemptyset(&siga.sa_mask);

    if(sigaction(SIGCONT, &siga, NULL) == -1) errExit("sigaction");

    printf("Signal on.\n");

    sleep(5);

    printf("Release mask\n");

    if(sigprocmask(SIG_UNBLOCK, &empty, NULL) == -1)
        errExit("sigprocmask");

    return 0;
}
