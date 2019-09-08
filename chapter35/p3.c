/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p3.c
*
* Author:           garyparrot
* Created:          2019/09/08
* Description:      exercise 3
******************************************************************************/
#define _GNU_SOURCE

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <sys/times.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    // setup realtime schedule, this property will inherit after fork()
    struct sched_param sp = { 10 };
    if(sched_setscheduler(0, SCHED_FIFO, &sp) == -1)
        errExit("sched_setschedulerr");
    
    // setup cpu affinity, this property will inherit after fork()
    cpu_set_t cset;
    CPU_ZERO(&cset);
    CPU_SET(0, &cset);
    if(sched_setaffinity(0, sizeof(cpu_set_t), &cset) == -1)
        errExit("sched_setaffinity");
    
    // fork
    pid_t child; 
    switch(child = fork()){
        case -1: errExit("fork");
        case  0: break;
        default: break;
    }

    alarm(10);

    // waste CPU, each 1/4 sec print message, each sec call sched_yield
    clock_t next = 0;
    clock_t next_switch = 0;
    for(;;){
        // calcuate current cpu time
        struct tms tm;
        if(times(&tm) == -1) errExit("times");

        clock_t cputime = tm.tms_utime + tm.tms_stime;

        if(cputime == next)
        {
            printf("[%5ld] Current time %lf\n", (long)getpid(), (double)cputime / sysconf(_SC_CLK_TCK));
            next += sysconf(_SC_CLK_TCK) / 4;
        }
        if(cputime == 3 * sysconf(_SC_CLK_TCK)) break;
        if(cputime == next_switch){
            printf("[%5ld] Switch\n", (long)getpid());
            next_switch += sysconf(_SC_CLK_TCK);
            sched_yield();
        }
    }

    return 0;
}
