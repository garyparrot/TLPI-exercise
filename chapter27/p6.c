/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p6.c
*
* Author:           garyparrot
* Created:          2019/07/31
* Description:      Exercise 6
******************************************************************************/

// cannot understand this exercise

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include "tlpi_hdr.h"

void handler(int sig){
    /* unsafe */
    printf("SIGCHLD\n");
}

int main(int argc, const char *argv[]){

    /* Setup disposition for SIGCHLD */
    struct sigaction siga;
    siga.sa_handler = handler;
    siga.sa_flags    = 0;
    sigemptyset(&siga.sa_mask);
    sigaction(SIGCHLD, &siga, NULL);

    /* Block it */
    sigset_t sg;
    sigemptyset(&sg);
    sigprocmask(SIG_BLOCK, &sg, NULL);

    /* fork */
    int childPid, status;
    switch(childPid = fork()){
        case -1: errExit("fork");
        case  0:
            _exit(EXIT_SUCCESS);
        default:
            // wait for it
            if(wait(&status) == -1)
                errExit("wait");
            // unblock 
            sigprocmask(SIG_UNBLOCK, &sg, NULL);
            break;
    }

    return 0;
}
