/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      Chapter 20 Exercise 2
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

int main(int argc, const char *argv[]){
    struct sigaction sgaction;
    sgaction.sa_handler = SIG_IGN;
    sigemptyset(&sgaction.sa_mask);
    sgaction.sa_flags   = 0;

    for(int i = 1; i < NSIG; i++)
        sigaction(i, &sgaction, NULL);

    pause();

    printf("This line will never ever get execute.");

    return 0;
}
