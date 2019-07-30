/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/30
* Description:      Exercise 1
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
    
    switch(fork()){
        case -1: errExit("fork");
        case  0: sleep(1); printf("%ld\n",(long)getppid()); _exit(EXIT_SUCCESS);
        default: exit(EXIT_SUCCESS);
    }
    return 0;
}
