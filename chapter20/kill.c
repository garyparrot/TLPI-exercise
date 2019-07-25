/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             kill.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      Kill
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <signal.h>
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    if(argc != 3 || 0 == strcmp(argv[1], "--help")) 
        usageErr("Usage %s signal-num pid", argv[0]);

    pid_t pid  = get_long(argv[2], GN_NUM_POSTIVE, GN_ANY_BASE);
    int signal = get_int(argv[1],  GN_NOFLAG, GN_ANY_BASE);

    // if pid is a postive number, it send signal to specific process
    // if pid is zero, it send signals to all its process group members
    // if pid is negative, it send signals to all the absolute value proces group members.
    // killpg(pgrp, sig) equal to kill(-pgrp, sig)
    int result = kill(pid, signal);

    if(result == -1){
        if(errno == EPERM)
            printf("Permission denied.\n");
        if(errno == ESRCH)
            printf("No such process %ld.\n", (long)pid);
        exit(EXIT_FAILURE);
    }

    // success 
    if(signal == 0)
        printf("Process %ld Exists\n", (long)pid);
    else
        printf("Success\n");
    
    return 0;
}
