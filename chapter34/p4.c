/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             disc_SIGHUP.c
*
* Author:           garyparrot
* Created:          2019/09/07
* Description:      Demonstrate controlling process terminiation will cause every member of foreground group receive SIGHUP
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

static void handler(int sig) {
    printf("PID %ld: caught signal %2d (%s)\n", (long)getpid(), sig, strsignal(sig));
}

int main(int argc, const char *argv[]){
    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s {d|s}... [ > sig.log 2>&1  ]\n", argv[0]);

    setbuf(stdout, NULL);


    pid_t parentPid = getpid();
    printf("PID of parent process is :          %ld\n", (long)parentPid);
    printf("Foreground process group ID is :    %ld\n", ((long)tcgetpgrp(STDIN_FILENO)));

    for(int j = 1;j < argc; j++){
        pid_t child;
        /* Fork a different process */
        if((child = fork()) == -1)
            errExit("fork");

        /* if this child process is marked as a d, create its own group */
        if(child == 0){
            if(argv[j][0] == 'd')
                if(setpgid(0,0) == -1)
                    errExit("setpgid");

            struct sigaction sa;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if(sigaction(SIGHUP, &sa, NULL) == -1)
                errExit("sigaction");
            break;  /* make this child process exit this for loop */
        }
    }

    /* Make this process immune to SIGHUP*/
    signal(SIGHUP, SIG_IGN);

    /* All process will terminate after 60 sec */
    alarm(60);

    printf("PID=%ld PGID=%ld\n", (long)getpid(), (long)getpgrp());

    for(;;)
        pause();

    return 0;
}
