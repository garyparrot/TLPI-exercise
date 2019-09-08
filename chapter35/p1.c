/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/09/08
* Description:      implement nice program
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

    if(argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s nice-value program [ args ... ]\n", argv[0]);

    int nice = get_int(argv[2], GN_NOFLAG, GN_BASE10);

    // set nice value in this program, after exec() the policy and priority is inherited.
    if(setpriority(PRIO_PROCESS, 0, nice) == -1) 
        errExit("setpriority");

    // exec new program
    if(execvp(argv[2], argv + 2) == -1)
        errExit("execvp");
    
    return 0;
}
