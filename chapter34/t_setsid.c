/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_setsid.c
*
* Author:           garyparrot
* Created:          2019/09/06
* Description:      Demonstrate how to create a new session
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

    /* setsid let you create a brand new session for this process */
    /* Since group leader cannot exit its own session, otherwise other process in the group will lose their leader */
    /* It is a little bit trick if process group leader want to create its own session */
    /* we have to fork a new process and let it create the session. the original process is useless, exit it. */

    if(fork() != 0)         /* exit if parent */
        _exit(EXIT_SUCCESS);

    if(setsid() == -1)     /*  */
        errExit("setsid");

    printf("PID=%ld, PGID=%ld, SID=%ld\n", (long)getpid(), (long)getpgrp(), (long)getsid(0));


    /* New session has no controlling terminal, we cannot open /dev/tty */
    if(open("/dev/tty", O_RDWR) == -1)
        errExit("open /dev/tty");
    exit(EXIT_SUCCESS);
    
    return 0;
}
