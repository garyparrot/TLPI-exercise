/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             sched_view.c
*
* Author:           garyparrot
* Created:          2019/09/08
* Description:      demonstrate how to get schedule policy and priority from process 
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    for(int j = 1; j < argc ; j++){
        int pol = sched_getscheduler(get_long(argv[j], GN_NOFLAG, GN_BASE10));
        if(pol == -1)
            errExit("sched_getscheduler");

        struct sched_param sp;

        if(sched_getparam(get_long(argv[j], GN_NOFLAG, GN_BASE10), &sp) == -1)
            errExit("sched_getparam");

        printf("%s: %-5s %2d\n", argv[j], 
                (pol == SCHED_OTHER) ? "OTHER" :
                (pol == SCHED_RR)    ? "RR" :
                (pol == SCHED_FIFO)  ? "FIFO" :
#ifdef SCHED_BATCH
                (pol == SCHED_BATCH) ? "BATCH" :
#endif
#ifdef SCHED_IDLE
                (pol == SCHED_IDLE)  ? "IDLE" :
#endif
                "???", sp.sched_priority);
    }

    exit(EXIT_SUCCESS);
}
