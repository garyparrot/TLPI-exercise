/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/09/07
* Description:      Exercise 2, demonstrate after exec, we cannot change the process group id of child process
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

    if(argc > 1 && strcmp(argv[1], "goto") == 0)
        goto there;

    printf("[%5ld] ready to fork\n", (long)getpid());

    switch(child = fork()){
        case -1:
            errExit("fork");
        case 0:
            if(argc > 1 && strcmp(argv[1], "exec") == 0)
                if(execl(argv[0], argv[0], "goto", NULL) == -1)
                    errExit("execl");
            sleep(3);
            break;
        default:
            sleep(1);
            printf("[%5ld] Ready to setpgid \n", (long)getpid());

            // change it immediately
            if(setpgid(child, child) == -1)
                errExit("setpgid");
            break;
    }

there:
    printf("[%5ld] my pid is %5ld, pgid = %5ld. ready to exit\n", (long)getpid(), (long)getpid(), (long)getpgrp());

    return 0;
}
