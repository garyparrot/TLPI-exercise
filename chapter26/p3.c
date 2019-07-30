/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             child_status.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Exercise 3
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
                siginfo_t infop;
                childPid = waitid(P_ALL, 0, &infop, WEXITED | WSTOPPED | WCONTINUED);

                if(childPid == -1)
                    errExit("waitid");

                printf("waitid() returned: PID=%ld\n", (long)infop.si_pid);
                switch(infop.si_code){
                    case CLD_EXITED:
                        printf("Process terminated by _exit(), return value %d\n", infop.si_status);
                        exit(EXIT_SUCCESS);
                    case CLD_KILLED:
                        printf("Process killed by signal %d\n", infop.si_status);
                        exit(EXIT_SUCCESS);
                    case CLD_STOPPED:
                        printf("Process stopped by signal %d\n", infop.si_status);
                        break;
                    case CLD_CONTINUED:
                        printf("Process continued by singal %d\n", infop.si_status);
                        break;
                    default:
                        printf("Unknow Event %ld \n", (long)infop.si_code);
                }


            }
    }
    
    return 0;
}
