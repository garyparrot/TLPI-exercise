/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_alarm.c
*
* Author:           garyparrot
* Created:          2019/07/28
* Description:      Demonstrating alarm()
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

void handler(int sig){
    printf("Receive signal\n");
}

int main(int argc, const char *argv[]){

    struct sigaction siga;
    siga.sa_handler = handler;
    siga.sa_flags   = 0;
    sigemptyset(&siga.sa_mask);

    sigaction(SIGALRM, &siga, NULL);
    
    // send SIGALRM after 5 sec
    // Linux makes alarm() and settitimer() share the same timer. Other platform might have different implementation since standard
    // didn't define how these function interaction.
    alarm(5);
    pause();
    printf("bye\n");
    return 0;
}
