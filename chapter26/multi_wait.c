/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             multi_wait.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Start multiple child process and wait until everybody terminated
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#include "tlpi_hdr.h"

char *currentTime(const char* fmt){
    static char buf[1024];

    time_t tm = time(NULL);
    strftime(buf,1024, fmt, localtime(&tm));
    return buf;
}

int main(int argc, const char *argv[]){

    pid_t childPid;

    if(argc < 2 || strcmp(argv[1], "--help") == 0) usageErr("%s sleep-time ...", argv[0]);

    setbuf(stdout, NULL);

    for(int i = 1;i < argc; i++){
        switch(fork()){
            case -1: errExit("fork");
            case  0:
                /* Let child process sleep for a while */
                printf("[%s] child %d started with PID %ld, sleeping %s seconds\n",
                        currentTime("%T"), i, (long) getpid(), argv[i]);
                sleep(get_int(argv[i], GN_NUM_NOT_ZERO, GN_ANY_BASE));
                _exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    int numDead = 0;
    for(;;){
        childPid = wait(NULL);
        if(childPid == -1){
            if(errno == ECHILD){
                printf("No more children - bye!\n");
                exit(EXIT_SUCCESS);
            }else{
                errExit("wait");
            }
        }

        numDead++;
        printf("[%s] wait() returned child PID %ld (numDead=%d)\n", currentTime("%T"), (long)childPid, numDead);
    }
    
    return 0;
}
