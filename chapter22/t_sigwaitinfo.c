/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_sigwaitinfo.c
*
* Author:           garyparrot
* Created:          2019/07/27
* Description:      Demonstrating sigwaitinfo(), wait until signal happens.
******************************************************************************/

#define _GNU_SOURCE

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

// You can replace sigwaitinfo by sigsuspend and custom handler,
// but with sigwaitinfo. it is much faster and convenience.

int main(int argc, const char *argv[]){

    if(argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [delay-secs]\n", argv[0]);

    printf("%s: PID is %ld\n", argv[0], (long)getpid());

    int sig;
    siginfo_t si;
    sigset_t allsig;

    sigfillset(&allsig);
    if(sigprocmask(SIG_SETMASK, &allsig, NULL) == -1)
        errExit("sigprocmask");
    printf("%s: signal blocked\n", argv[0]);

    if(argc > 1){
        printf("%s: about to delay %s seconds \n", argv[0], argv[1]);
        sleep(get_int(argv[1], GN_NUM_NOT_ZERO, GN_BASE10));
        printf("%s: finished delay\n", argv[0]);
    }

    for(;;){
        sig = sigwaitinfo(&allsig, &si);
        if(sig == -1) errExit("sigwaitinfo");

        if(sig == SIGINT || sig == SIGTERM)
            exit(EXIT_SUCCESS);

        printf("Got signal %d, code=%d (%s) si_value=%d\n", si.si_signo, si.si_code, 
                (si.si_code == SI_USER) ? "SI_USER" :
                (si.si_code == SI_QUEUE) ? "SI_QUEUE" : "other",
                si.si_value.sival_int);

        printf("    si_pid=%ld, si_uid=%ld\n", (long)si.si_pid, (long)si.si_uid);
    }
    
    return 0;
}
