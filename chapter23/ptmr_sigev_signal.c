/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             ptmr_sigev_signal.c
*
* Author:           garyparrot
* Created:          2019/07/28
* Description:      Demonstrating POSIX Timer API
******************************************************************************/

#define _POSIX_C_SOURCE 199309
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include "itimerspec_from_str.h"
#include "tlpi_hdr.h"

#define TIMER_SIG SIGRTMAX

static void handler(int sig, siginfo_t *si, void *uc){
    timer_t *tidptr;

    /* Get singal payload */
    tidptr = si->si_value.sival_ptr;

    struct tm res;
    time_t tim = time(NULL);
    char buffer[256];
    strftime(buffer, 256, "%T", localtime_r(&tim, &res));

    printf("[%s] Got signal %d\n", buffer , sig);
    printf("    *sival_ptr          =%ld\n", (long) *tidptr);
    printf("    *timer_getoverrun() =%d\n", timer_getoverrun(*tidptr));
}

int main(int argc, char *argv[]){

    if(argc < 2) usageErr("%s secs[/nsecs][:int-secs[/int-nsecs]]...\n", argv[0]);

    timer_t *tidlist;

    tidlist = calloc(argc -1, sizeof(timer_t));
    
    if(tidlist == NULL) errExit("malloc");

    struct sigaction sa;
    sa.sa_flags     = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if(sigaction(TIMER_SIG, &sa, NULL) == -1) errExit("malloc");

    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo  = TIMER_SIG;

    struct itimerspec ts;
    for(int i = 0;i < argc - 1; i++){
        itimerspecFromStr(argv[i + 1], &ts);

        sev.sigev_value.sival_ptr = &tidlist[i];

        if(timer_create(CLOCK_REALTIME, &sev, &tidlist[i]) == -1)
            errExit("timer_create");
        printf("Timer ID: %ld (%s)\n", (long) tidlist[i], argv[i+1]);

        if(timer_settime(tidlist[i], 0, &ts, NULL) == -1)
            errExit("timer_settime");
    }

    for(;;)
        pause();
    
    return 0;
}
