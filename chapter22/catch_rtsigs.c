/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             catch_rtsigs.c
*
* Author:           garyparrot
* Created:          2019/07/27
* Description:      Demonstrating catching real time signal
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

static volatile int handerSleepTime;
static volatile int sigCnt = 0;
static volatile int allDone = 0;

static void handler(int sig, siginfo_t *si, void *ucontext){
    if(sig == SIGINT || sig == SIGTERM){
        allDone = 1;
        return;
    }

    sigCnt++;
    printf("caught signal %d\n", sig);

    printf("    si_signo=%d, si_code=%d (%s)", si->si_signo, si->si_code, 
           (si->si_code == SI_USER) ? "SI_USER" :
           (si->si_code == SI_QUEUE)? "SI_QUEUE" : "OTHER");
    printf("si_value=%d\n", si->si_value.sival_int);
    printf("    si_pid=%ld, si_uid=%ld\n", (long) si->si_pid, (long) si->si_uid);

    sleep(handerSleepTime);
}

int main(int argc, const char *argv[]){

    if(argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [block-time [handler-sleep-time]]", argv[0]);

    printf("%s: PID is %ld\n", argv[0], (long)getpid());

    handerSleepTime = (argc > 2) ? get_int(argv[2], GN_NUM_POSTIVE, GN_ANY_BASE) : 1;

    struct sigaction sa;

    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;
    sigfillset(&sa.sa_mask);

    for(int s = 1; s < NSIG; s++)
        if(s != SIGTSTP && s != SIGQUIT)
            sigaction(s, &sa, NULL);

    sigset_t prevMask, blockMask;
    if(argc > 1){
        sigfillset(&blockMask);
        sigdelset(&blockMask, SIGINT);
        sigdelset(&blockMask, SIGTERM);

        if(sigprocmask(SIG_SETMASK, &blockMask, &prevMask) == -1)
            errExit("sigprocmask");

        printf("%s: signals blocked - sleeping %s second\n", argv[0], argv[1]);
        sleep(get_int(argv[1], GN_NUM_NOT_ZERO, GN_ANY_BASE));
        printf("%s: sleep complete\n", argv[0]);

        if(sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
            errExit("sigprocmask");

        while(!allDone)
            pause();
    }

    return 0;
}
