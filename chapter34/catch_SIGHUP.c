/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             catch_SIGHUP.c
*
* Author:           garyparrot
* Created:          2019/09/06
* Description:      Simulate that shell will send SIGHUP to every process group create by itself
*                   Note this doesn't count those escaped by setpgid() function
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

static void handler(int sig) {}

int main(int argc, const char *argv[]){

    setbuf(stdout, NULL);       /* Make stdout unbuffered */

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if(sigaction(SIGHUP, &sa, NULL) == -1)
        errExit("sigaction");

    pid_t child = fork();

    /* if argument specified, child process create its own process group */
    if(child == 0 && argc > 1)
        if(setpgid(0, 0) == -1)
            errExit("setpgid");

    printf("PID=%ld; PPID=%ld; PGID=%ld; SID=%ld\n",
            (long)getpid(), (long)getppid(), (long)getpgrp(), (long)getsid(0));

    alarm(60);

    for(;;){
        pause();        /* wait for signal */
        printf("%ld: caught SIGHUP\n", (long)getpid());
    }
    
    return 0;
}
