/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             show_sigset.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      Signal receiver
******************************************************************************/

/* This experiment shows that for each signal, kernel won't record signal amount */
/* If there is multiple signal shot to a process, the process might get less signal then sender does */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <signal.h>

#include "tlpi_hdr.h"
#include "signal_functions.h"

static int sigCnt[NSIG];
static volatile sig_atomic_t gotSigint= 0;

static void handler(int sig){
    if( sig == SIGINT )
        gotSigint = 1;
    else
        sigCnt[sig]++;
}

int main(int argc, const char *argv[]){

    printf("%s: PID is %ld\n", argv[0], (long)getpid());

    // sigaction has better portability
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    for(int n = 1; n < NSIG;n++)
    {
        (void)sigaction(n, &action, NULL);
    }

    if(argc > 1){
        int numSecs = get_int(argv[1], GN_NOFLAG, GN_ANY_BASE);
        sigset_t sigMask;
        sigset_t sigPending;

        sigfillset(&sigMask);
        if(sigprocmask(SIG_SETMASK,&sigMask, NULL) == -1)
            errExit("sigprocmask");

        printf("%s: sleeping for %d seconds\n", argv[0], numSecs);
        sleep(numSecs);


        if(sigpending(&sigPending) == -1)
            errExit("sigpending");

        printf("%s: pending signals are: \n", argv[0]);
        printSigset(stdout, "\n\n", &sigPending);

        sigemptyset(&sigMask);
        if(sigprocmask(SIG_SETMASK, &sigMask, NULL) == -1)
            errExit("sigprocmask");
    }

    while(!gotSigint)
        continue;

    for(int n = 1; n < NSIG; n++)
        if(sigCnt[n] != 0)
            printf("%s: signal %d caught %d times %s\n", argv[0], n, sigCnt[n], (sigCnt[n] == 1) ? "" : "s");

    return 0;
}
