/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_sigsuspend.c
*
* Author:           garyparrot
* Created:          2019/07/27
* Description:      Demonstrating sigsuspend()
******************************************************************************/

// atomic operation of sigprocmask & pause

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include "signal_functions.h"
#include "tlpi_hdr.h"

static volatile sig_atomic_t gotSigquit = 0;

static void handler(int sig){
    printf("Caught signal %d (%s)\n", sig, strsignal(sig));

    if(sig == SIGQUIT)
        gotSigquit = 1;
}

int main(int argc, const char *argv[]){

    int loopNum;
    time_t stime;
    sigset_t origMask, blockMask;
    struct sigaction sa;

    printSigMask(stdout, "Initial signal mask is:\n");

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGINT);
    sigaddset(&blockMask, SIGQUIT);
    if(sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
        errExit("sigprocmask - SIG_BLOCK");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if(sigaction(SIGINT, &sa, NULL) == -1) errExit("sigaction");
    if(sigaction(SIGQUIT, &sa, NULL) == -1) errExit("sigaction");

    for(loopNum = 1; !gotSigquit; loopNum++){

        printf("=== LOOP %d\n", loopNum);

        printSigMask(stdout, "Starting critical section, signal mask is:\n");

        for(stime = time(NULL); time(NULL) < stime + 4;)
            continue;

        printPendingSigs(stdout, "Before sigsuspend() - pending signals:\n");

        // instead of using sigprocmask + pause
        // sometime the signal shot too fast, the pause didn't get executed.
        // the signal already been done.
        //
        // cause pause() loop forever
        //
        // use sigsuspend for atomic operation of sigprocmask + pause
        if(sigsuspend(&origMask) == -1 && errno != EINTR)
            errExit("sigsuspend");

    if(sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
        errExit("sigprocmask - SIG_SETMASK");

    printSigMask(stdout, "=== Exited loop\nRestore signal mask to: \n");

    return 0;
}
