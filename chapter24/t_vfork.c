/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_vfork.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Demonstrating v_fork()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

/* for vfork, child process always go first */

/* vfork is different from fork */
/* vfork share memory with parent process */
/* parent wait for child process exit */

/* vfork makes parent and child process share memory */
/* vfork is faster since it doesn't copy pages */
/* Abuse vfork might cause wired behavior. if speed is not the metric, use fork instead vfork.  */

int main(int argc, const char *argv[]){

    int istack = 222;

    switch(vfork()){
        case -1:
            errExit("vfork");

        case 0:
            sleep(3);

            write(STDOUT_FILENO, "Child executing\n", 16);
            istack *= 3;
            _exit(EXIT_SUCCESS);

        default:
            write(STDOUT_FILENO, "Parent executing\n", 17);
            printf("istack=%d\n", istack);
            exit(EXIT_SUCCESS);
    }
    
    return 0;
}
