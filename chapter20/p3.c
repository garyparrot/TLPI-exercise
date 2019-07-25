/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p3.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      Chapter 20 Exercise 3
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

void Signal_Quit(int i){
    printf("You sure you want to do that?\n");
}

static volatile int nest_cnt = 0;

void Signal_Interrupt(int i){
    nest_cnt++;

    if(nest_cnt > 2)
        printf("How dare you bother me ??? Leave me alone.\n");
    else
    {
        sleep(1);
        printf("...\n");
    }

    nest_cnt--;
}

// First time you send SIGQUIT, the notification shows up. Next time SIGQUIT will actually perform its originaly behavior.
// SIGINT three times within 1 second will put a line on the screen tell you to stop.

int main(int argc, const char *argv[]){

    setvbuf(stdout, NULL, _IONBF, 0);

    struct sigaction siga;
    siga.sa_handler = Signal_Interrupt;
    sigemptyset(&(siga.sa_mask));
    siga.sa_flags   = SA_NODEFER;

    sigaction(SIGINT, &siga, NULL);

    siga.sa_handler = Signal_Quit;
    sigemptyset(&(siga.sa_mask));
    siga.sa_flags   = SA_RESETHAND;

    sigaction(SIGQUIT, &siga, NULL);

    for(;;);
    
    return 0;
}
