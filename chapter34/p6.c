/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p6.c
*
* Author:           garyparrot
* Created:          2019/09/07
* Description:      Exercise 6, demonstrate that orphaned process group read() cause EIO error
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

    pid_t child;

    switch(child = fork()){
        case -1:
            errExit("fork");
        case 0:
            sleep(1);       /* Wait a sec */
            if(read(STDIN_FILENO, NULL, 0) == -1)
                errExit("read");
            break;
        default:
            /* Just exit */
            _exit(EXIT_SUCCESS);
            break;
    }
    
    return 0;
}
