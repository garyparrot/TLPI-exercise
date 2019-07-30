/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             child_status.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

#ifdef WCONTINUED
#define wait_pid_option (WUNTRACED | WCONTINUED)
#else
#define wait_pid_option (WUNTRACED)
#endif

int main(int argc, const char *argv[]){

    int status;
    pid_t childPid;

    if(argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [exit-status]\n", argv[0]);

    switch(fork()){
        case -1: errExit("fork");
        case  0:
            printf("Child started with PID= %ld\n", (long)getpid());
            /* If return code is specified, just return it */
            /* Otherwise pause() forever */
            if( argc > 1 )
                exit(get_int(argv[1], GN_NOFLAG, GN_ANY_BASE));
            else
                for(;;)
                    pause();
            exit(EXIT_FAILURE);
        default:
            for(;;){
                childPid = waitpid(-1, &status, wait_pid_option);

                if(childPid == -1)
                    errExit("waitpid");

                printf("waitpid() returned: PID=%ld; status=0x%04x (%d,%d)\n", (long)childPid, 
                        (unsigned int) status, status >> 8, status & 0xff);
                printWaitStatus(NULL, status);

                if(WIFEXITED(status) || WIFSIGNALED(status))
                    exit(EXIT_SUCCESS);

            }
    }
    
    return 0;
}
