/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             ouch.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      Demonstrating signal()
******************************************************************************/

#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

static void sigHandler(int sig){
    printf("Ouch!\n");
}

int main(int argc, const char *argv[]){

    if(signal(SIGINT, sigHandler) == SIG_ERR)
        errExit("signal");
    
    for(int j = 0; ; j++){
        printf("%d\n", j);
        sleep(3);
    }
    
    return 0;
}
