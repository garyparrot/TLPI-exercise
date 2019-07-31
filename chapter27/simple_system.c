/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             simple_system.c
*
* Author:           garyparrot
* Created:          2019/07/31
* Description:      Demonstrate system()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "tlpi_hdr.h"

int System(const char *command){

    int status;
    pid_t childPid;

    switch(childPid = fork()){
        case -1: 
            return -1;
        case  0: /* child */
            execl("/bin/sh", "sh", "-c", command, (char*)NULL);
            _exit(127);
        default:
            if(waitpid(childPid, &status, 0) == -1)
                return -1;  /* return -1 on error */
    }
    return status; /* successful, return the result */
}

int main(int argc, const char *argv[]){
    printf("Return value: %d\n", System(argv[1]));
}
