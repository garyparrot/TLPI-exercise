/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Exercise 2
******************************************************************************/

// shows that vfork() process won't affect parent's file descriptor.

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

    int frk;
    switch(frk = vfork()){
        case -1: errExit("vfork");
        case  0:
            write(STDOUT_FILENO,"About to close the fd.\n",23);
            close(STDOUT_FILENO);
            write(STDOUT_FILENO, "Muted???\n", 9);
            _exit(EXIT_SUCCESS);
        default:
            wait(NULL);

            /* Still working */
            write(STDOUT_FILENO, "Hello World\n", 12);
            exit(EXIT_SUCCESS);
    }
    
    return 0;
}
