/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_setpriority.c
*
* Author:           garyparrot
* Created:          2019/09/08
* Description:      demonstrate getpriority and setpriority
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    if(argc != 4 || strchr("pgu", argv[1][0]) == NULL)
        usageErr("%s {p|u|g} who priority\n"
                 "   set priority of: p=process; g=process group;"
                 "u=processes for user\n", argv[0]);

    int which = (argv[1][0] == 'p') ? PRIO_PROCESS :
                (argv[1][0] == 'g') ? PRIO_PGRP : PRIO_USER;
    int who   = get_long(argv[2], GN_NOFLAG, GN_BASE10);
    int prio  = get_int(argv[3], GN_NOFLAG, GN_BASE10);

    if(setpriority(which, who, prio) == -1)
        errExit("getpriority");

    errno = 0;
    prio = getpriority(which, who);
    if(prio == -1 && errno != 0)
        errExit("getpriority");

    printf("Nice value = %d\n", prio);

    exit(EXIT_SUCCESS);
}
