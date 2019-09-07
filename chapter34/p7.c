/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p7.c
*
* Author:           garyparrot
* Created:          2019/09/07
* Description:      demonstrate that sending SIGTTIN, SIGTTOU, SIGTSTP to member of orphaned group cause following behavior
*                   1. if SIG_DFL is specified for signal, nothing happened
*                   2. if handler is setted, handler get executed
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

static void handler(int sig){
    printf("Process %ld receive signal %d(%s)\n", (long)getpid(), sig, strsignal(sig));
}

int main(int argc, const char *argv[]){

    pid_t child;

    setbuf(stdout, NULL);

    switch(child = fork()){
        case -1:
            errExit("fork");
        case 0:
            // isolate myself
            if(setpgid(0,0) == -1) errExit("setpgid");

            // if a argument is specified, setup handler for it
            if(argc > 1) {
                struct sigaction sa;            
                sa.sa_flags = 0;
                sa.sa_handler = handler;
                sigfillset(&sa.sa_mask);

                sigaction(SIGTTIN, &sa, NULL);
                sigaction(SIGTTOU, &sa, NULL);
                sigaction(SIGTSTP, &sa, NULL);
            }

            // keep receive signal
            for(;;){ pause(); }

            break;
        default:
            /* Sleep for a while then send the freaking signals */
            sleep(1);

            kill(child, SIGTTIN);
            kill(child, SIGTTOU);
            kill(child, SIGTSTP);

            break;
    }
    
    return 0;
}
