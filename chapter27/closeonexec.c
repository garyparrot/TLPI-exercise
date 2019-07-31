/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             closeonexec.c
*
* Author:           garyparrot
* Created:          2019/07/30
* Description:      Demonstrating Close on exec flag.
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    int flags;

    /* If argument is given, raies CLOEXEC flag */
    if(argc > 1){
        /* Not every flag can be change by fcntl, some of flag is fixed since it creat. */
        flags = fcntl(STDOUT_FILENO, F_GETFD);
        if(flags == -1) errExit("fcntl - F_GETFD");

        flags |= FD_CLOEXEC;

        if(fcntl(STDOUT_FILENO, F_SETFD, flags) == -1)
            errExit("fcntl - FSETFD");

        /* Execute ls command */
        execlp("ls", "ls", "-l", argv[0], (char*)NULL);

        errExit("execlp");
    }
    
    return 0;
}
