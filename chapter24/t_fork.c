/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_fork.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Demonstrating fork()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

static int idata = 111;


/* fork process share memory with COW technique. */
/* A few trick can be perform with fork, e.g. prevent memory leak function afftect parent process */
int main(int argc, const char *argv[]){

    int istack = 222;
    pid_t childPid;

    /* You cannnot assume which process will go first */
    switch(childPid = fork()){
        case -1:
            errExit("fork");
        case 0:
            idata *= 3;
            istack*= 3;
            break;

        default:
            sleep(3);
            break;
    }

    printf("PID=%ld %s idata=%d istack=%d\n", (long)getpid(), (childPid == 0) ? "Child" : "Parent", idata, istack);
    
    return 0;
}
