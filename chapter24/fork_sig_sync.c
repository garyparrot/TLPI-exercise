/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             fork_sig_sync.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Demonstrating how to sync two process via signal.
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include "tlpi_hdr.h"

#define SYNC_SIG SIGUSR1

static void handler(int sig){}

static const char* currTime(const char* fmt){
    static char buffer[1024];

    time_t Etime = time(NULL);

    strftime(buffer, 1024, fmt, localtime(&Etime));

    return buffer;
}

int main(int argc, const char *argv[]){

    pid_t childPid;

    setbuf(stdout, NULL);

    // signal mask
    sigset_t blockMask, origMask, emptyMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SYNC_SIG);
    if(sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
        errExit("sigprocmask");

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if(sigaction(SYNC_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    switch (childPid = fork()) {
        case -1:
            errExit("fork");
        case 0: /* Child */


            printf("[%s %ld] Child started, soing some work\n", currTime("%T"), (long)getpid());

            /* Simulate time spent doing some work */
            sleep(2);

            printf("[%s %ld] Child about to signal parent\n", currTime("%T"), (long)getpid());

            /* Send signal to parent */
            if(kill(getppid(), SYNC_SIG) == -1) errExit("kill");

            _exit(EXIT_SUCCESS);

        default:
            
            printf("[%s %ld] Parent about to wait for signal\n", currTime("%T"), (long)getpid());

            sigemptyset(&emptyMask);

            if(sigsuspend(&emptyMask) == -1 && errno != EINTR)
                errExit("sigsuspend");

            printf("[%s %ld] Parent got signal\n", currTime("%T"), (long)getpid());

            if(sigprocmask(SIG_SETMASK, &origMask, NULL) == -1) errExit("sigprocmask");

            exit(EXIT_SUCCESS);
    }
    
    return 0;
}
