/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p3.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Chapter 23 Exercise 3
******************************************************************************/

// I am not sure I do this right
//
// My test result shows si_value.sival_int not equal to timerID

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

int main(int argc, const char *argv[]){

    /* Mask SIGALRM */
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGALRM);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    
    /* Creat Timer */
    struct itimerspec tsp = { 
        .it_value    = { .tv_sec = 1, .tv_nsec = 0 },
        .it_interval = { .tv_sec = 1, .tv_nsec = 0 }
    };
    timer_t tid;
    printf("Timer handler %p\n", tid);
    if(timer_create(CLOCK_REALTIME, NULL, &tid) == -1) errExit("timer_create");
    if(timer_create(CLOCK_REALTIME, NULL, &tid) == -1) errExit("timer_create");
    if(timer_create(CLOCK_REALTIME, NULL, &tid) == -1) errExit("timer_create");
    if(timer_settime(tid, 0, &tsp, NULL)) errExit("timer_settime");

    /* wait for the signal */
    for(;;){
        siginfo_t info;

        sigwaitinfo(&sigset, &info);

        printf("Receive signal %s, timer_id %d, payload %d\n",
                (info.si_signo == SIGALRM ? "SIGALRM" : "OTHER"),
                info.si_timerid,
                info.si_value.sival_int);
    }

    return 0;
}
