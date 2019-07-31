/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p4.c
*
* Author:           garyparrot
* Created:          2019/07/31
* Description:      Exercise 4
******************************************************************************/

/* Grandpa process always exit successful. */
/* Grandchild process will be a orphan. and after it exit. init will dispose it automatically */
/* This is useful when you have to do something with fork but you cannot wait for it. This method could prevent zombie process */
/* Since SUSv3 doesn't guarantee how to do with SIGCHLD after exec(). (Some implementation keep it ignored, some reset it to default)  */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){
    
    int status;
    int childPid = fork();

    if(childPid == -1)
        errExit("fork1");
    if(childPid == 0){
        switch(fork()){
            case -1: errExit("fork2");
            case  0: 
                /* Do something important */
                exit(EXIT_SUCCESS);
            default:
                exit(EXIT_SUCCESS);
        }
    }

    /* wait for it */
    if(waitpid(childPid, &status, 0) == -1)
        errExit("waitpid");
    return 0;
}
