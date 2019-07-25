/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p4.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      Chapter 20 Exercise 4
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

int siginterrupt(int sig, int flag){
    struct sigaction sig_conf;
    if(sigaction(sig, NULL, &sig_conf) == -1) return -1;

    if(flag == 0)
        sig_conf.sa_flags |= SA_RESTART;
    else
        sig_conf.sa_flags &= ~(SA_RESTART);

    if(sigaction(sig, &sig_conf, NULL) == -1) return -1;

    return 0;
}

int main(int argc, const char *argv[]){

    return 0;
}
