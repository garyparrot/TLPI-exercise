/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/09/11
* Description:      exercise 2
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s command arg ...", argv[0]);

    pid_t child;
    switch(child = fork()){
        case -1:
            errExit("fork");
        case  0:
            if(execvp(argv[1], argv + 1) == -1)
                errExit("execvp");
            _exit(EXIT_FAILURE);
        default:
            break;
    }

    int wstatus; 

    if(waitpid(child, &wstatus, 0) == -1)
        errExit("waitpid");

    struct rusage r_usage;
    if(getrusage(RUSAGE_CHILDREN, &r_usage) == -1)
        errExit("getrusage");

    printf("user  %8.6lfs\n", (r_usage.ru_utime.tv_sec * 1e6 + r_usage.ru_utime.tv_usec) / 1e6);
    printf("sys   %8.6lfs\n", (r_usage.ru_stime.tv_sec * 1e6 + r_usage.ru_stime.tv_usec) / 1e6);
    printf("total %8.6lfs\n", (
                                (r_usage.ru_utime.tv_sec+r_usage.ru_stime.tv_sec) * 1e6 +
                                (r_usage.ru_utime.tv_usec+r_usage.ru_stime.tv_usec) ) / 1e6);
    
    return 0;
}
