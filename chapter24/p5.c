/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             fork_sig_sync.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Exercise 5.
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

            printf("[%s %ld] Child Finish first part of job, wait for parent done its part.\n", currTime("%T"), (long)getpid());

            /* Wait for parent doing its job*/
            sigemptyset(&emptyMask);
            if(sigsuspend(&emptyMask) == -1 && errno != EINTR)
                errExit("sigsuspend");
            
            printf("[%s %ld] Child Deal with Second part of job.\n", currTime("%T"), (long)getpid());

            sleep(2);

            printf("[%s %ld] Child about to signal parent\n", currTime("%T"), (long)getpid());

            /* Send signal to parent */
            if(kill(getppid(), SYNC_SIG) == -1) errExit("kill");

            _exit(EXIT_SUCCESS);

        default:

            printf("[%s %ld] Parent doing its job...\n", currTime("%T"), (long)getpid());
            sleep(3);
            printf("[%s %ld] Parent Might takes some time...\n", currTime("%T"), (long)getpid());
            sleep(3);
            printf("[%s %ld] Parent Almost there...\n", currTime("%T"), (long)getpid());
            sleep(2);
            printf("[%s %ld] Parent done. \n", currTime("%T"), (long)getpid());

            /* Notify child process */
            kill(childPid, SYNC_SIG);
            
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
