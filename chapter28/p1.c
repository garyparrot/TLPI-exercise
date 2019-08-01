/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/08/01
* Description:      Exercise 1
******************************************************************************/

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
    
    if(argc <= 1 || strcmp(argv[1], "--help") == 0)
        usageErr("%s (fork | vfork) [ times ]", argv[0]);

    int operation = (strcmp(argv[1], "fork") == 0) ? 1 : 
                    (strcmp(argv[1], "vfork") == 0) ? 2 : 0;
    int times = (argc > 2) ? (atoi(argv[2])) : 1000000;

    if(operation == 0) fatal("Wrong operation '%s', it should be fork or vfork");

    int status;

    for(int i = 0;i < times; i++){
        switch((operation == 1) ? fork() : vfork()){
            case -1: errExit((operation == 1) ? "fork" : "vfork");
            case  0:
                _exit(EXIT_SUCCESS);
            default:
                if(wait(&status) == -1)
                    errExit("wait");
        }
    }

    return 0;
}
