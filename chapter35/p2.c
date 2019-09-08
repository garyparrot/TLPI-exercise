/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/09/08
* Description:      implement nice program ( with realtime schedule support )
******************************************************************************/
#define _GNU_SOURCE

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>
#include <sched.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    if(argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s {policy , f|r|o} priority command [ args ... ]\n", argv[0]);

    int policy = argv[1][0] == 'f' ? SCHED_FIFO :
                 argv[1][0] == 'r' ? SCHED_RR   :
                 argv[1][0] == 'o' ? SCHED_OTHER : -1100;

    struct sched_param sp;
    sp.sched_priority = get_int(argv[2], GN_NOFLAG, GN_BASE10);

    if(sched_setscheduler(0, policy, &sp) == -1 && errno )
        errExit("sched_setscheduler");

    // strip root privilege
    int ruid = getuid();
    if(setresuid(ruid, ruid, ruid) == -1)
        errExit("setresuid");

    // exec new program
    if(execvp(argv[3], argv + 3) == -1)
        errExit("execvp");
    
    return 0;
}
