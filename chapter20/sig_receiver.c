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

void printSigset(FILE *of, const char *prefix, const sigset_t *sigset){
    int cnt = 0;

    for(int sig = 1; sig < NSIG; sig++){
        if(sigismember(sigset, sig)){
            cnt++;
            fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
        }
    }

    if(cnt == 0)
        fprintf(of, "%s<empty signal set>\n", prefix);
}

/* Show blocked signal for this process */
int printSigMask(FILE *of, const char *msg){
    sigset_t currMask;

    if(msg != NULL) fprintf(of, "%s", msg);

    if(sigprocmask(SIG_BLOCK, NULL, &currMask) == -1)
        return -1;

    printSigset(of, "\t\t", &currMask);

    return 0;
}

/* Show pending signal */
int printPendingSigs(FILE *of, const char *msg){
    sigset_t pendingSigs;

    if(msg != NULL) fprintf(of, "%s", msg);

    if(sigpending(&pendingSigs) == -1)
        return -1;

    printSigset(of, "\t\t", &pendingSigs);

    return 0;
}

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

    for(int n = 1; n < NSIG;n++)
        /* The result to convert the return value to void, means I know the function has return value but I want to ignore it */
        /* This behavior also might started from making code analyzer shut up */
        (void)signal(n, handler);   

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
