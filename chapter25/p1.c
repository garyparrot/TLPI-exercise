/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Exercise 1
******************************************************************************/

// 255

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

    int wstatus;

    switch(fork()){
        case -1: errExit("fork");
        case  0:
            exit(-1);
        default:
            wait(&wstatus);
            printf("Child return state = %d\n", WEXITSTATUS(wstatus));
    }
    
    return 0;
}
