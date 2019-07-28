/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_realtime.c
*
* Author:           garyparrot
* Created:          2019/07/28
* Description:      Demonstrating setitimer() getitimer()
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
#include <sys/time.h>

#include "tlpi_hdr.h"

static volatile sig_atomic_t gotSignal = 0;

static void display(const char *msg, int includeTimer){
    static struct timeval start, curr; 
    static int callNum = 0;

    if(callNum == 0)
        if(gettimeofday(&start, NULL) == -1)
            errExit("gettimeofday");

    /* show header every 20 line */
    if(callNum % 20 == 0)
        printf("          Elap    Value   Interval\n");

    if(gettimeofday(&curr, NULL) == -1)
        errExit("gettimeofday");

    printf("%-7s %6.2f", msg, curr.tv_sec - start.tv_sec + (curr.tv_usec - start.tv_usec) / 1000000.0);

    if(includeTimer){
        static struct itimerval itv;
        if(getitimer(ITIMER_REAL, &itv) == -1)
            errExit("getitimer");
        printf("  %6.2f  %6.2f", 
                itv.it_value.tv_sec + itv.it_value.tv_usec / 1000000.0,
                itv.it_interval.tv_sec + itv.it_interval.tv_usec / 1000000.0);
    }

    printf("\n");
    callNum++;
}

void handler(int sig){ gotSignal = 1; }

int main(int argc, const char *argv[]){
    
    /* Init args */
    if(argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [secs [usecs [int-secs[int-usecs]]]]", argv[0]);

    /* Setup signal handler */
    struct sigaction siga;
    siga.sa_handler = handler;
    siga.sa_flags   = 0;
    sigemptyset(&siga.sa_mask);

    if(sigaction(SIGALRM, &siga, NULL) == -1)
        errExit("sigaction");

    /* Setup real time timer*/
    time_t times            = (argc > 1) ? get_long(argv[1], GN_NUM_POSTIVE, GN_BASE10) : 2;
    suseconds_t utimes      = (argc > 2) ? get_long(argv[2], GN_NUM_POSTIVE, GN_BASE10) : 0;
    time_t init_times       = (argc > 3) ? get_long(argv[3], GN_NUM_POSTIVE, GN_BASE10) : 0;
    suseconds_t init_utimes = (argc > 4) ? get_long(argv[4], GN_NUM_POSTIVE, GN_BASE10) : 0;

    struct itimerval itv = {
        .it_interval = { .tv_sec = times, .tv_usec = utimes },
        .it_value    = { .tv_sec = init_times, .tv_usec = init_utimes }
    }; 

    /* setitimer set a interval timer */
    /* there is three kind of timer
     * 1. real time timer
     * 2. virtual user mode timer
     * 3. profiling timer (use + sys time)*/
    if(setitimer(ITIMER_REAL, &itv , NULL) == -1)
        errExit("setitimer");

    clock_t prevClock;

    int maxSig = (itv.it_value.tv_sec == 0 && itv.it_value.tv_usec == 0) ? 1 : 3;

    int sigCnt = 0;

    display("START:", 0);

    /* loop */
    for(;;){
        /* Continues checking is the signal on. */
        /* within 0.5 second */
        while(((clock() - prevClock) * 10 / CLOCKS_PER_SEC) < 5){
            if(gotSignal){
                gotSignal = 0;
                display("ALARM:", 1);

                sigCnt++;
                if(sigCnt >= maxSig){
                    printf("Bye\n");
                    exit(EXIT_SUCCESS);
                }
            }
        }

        /* Main function reporter, steady report status every 0.5sec */
        /* show status */
        prevClock = clock(); 
        display("Main: ", 1);
    }
    return 0;
}
