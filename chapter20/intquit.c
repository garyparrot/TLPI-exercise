/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             intquit.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "tlpi_hdr.h"

static void handler(int sig){
    if(sig == SIGINT){
        printf("Interrupt\n");
        return;
    }

    printf("Bye!");
    exit(EXIT_SUCCESS);
}

int main(int argc, const char *argv[]){

    if(signal(SIGINT, handler) == SIG_ERR) errExit("signal");
    if(signal(SIGQUIT, handler) == SIG_ERR) errExit("signal");

    for(;;)
        pause();

    
    return 0;
}
