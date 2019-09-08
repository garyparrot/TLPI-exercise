/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             sched_set.c
*
* Author:           garyparrot
* Created:          2019/09/08
* Description:      demonstrate how to change scheduling policy
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
    
    if (argc < 3 || strchr("rfo", argv[1][0]) == NULL)
        usageErr("%s policy priority [pid...]\n"
                 "   policy is 'r'(RR), 'f'(FIFO), "
#ifdef SCHED_BATCH
                 "'b' (BATCH), "
#endif
#ifdef SCHED_IDLE
                 "'i' (IDLE), "
#endif
                 " or 'o' (OTHER)\n", argv[0]
                 );

    int pol = (argv[1][0] == 'r') ? SCHED_RR :
              (argv[1][0] == 'f') ? SCHED_FIFO :
#ifdef SCHED_BATCH
              (argv[1][0] == 'b') ? SCHED_BATCH :
#endif
#ifdef SCHED_IDLE
              (argv[1][0] == 'i') ? SCHED_IDLE :
#endif
              SCHED_OTHER;

    struct sched_param sp;
    sp.sched_priority = get_int(argv[2], GN_NOFLAG, GN_BASE10);

    for(int j = 3;j < argc; j++)
        if(sched_setscheduler(get_long(argv[j], GN_NOFLAG, GN_BASE10), pol, &sp) == -1)
            errExit("sched_setscheduler");

    exit(EXIT_SUCCESS);
}
