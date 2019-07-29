/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p3.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Exercise 3
******************************************************************************/

// How to generate core dump and keep the process going in the mean time.

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    /* fork one for core dump */
    switch(fork()){
        case -1:
            errExit("fork");
        case  0:
            raise(SIGQUIT);
    }

    /* main process */
    printf("I am alive!\n");
    
    return 0;
}
