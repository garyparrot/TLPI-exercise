/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_sigtimedwait.c
*
* Author:           garyparrot
* Created:          2019/07/27
* Description:      Demonstrating sigtimedwait()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    // 5sec
    struct timespec ts = { .tv_sec = 5, .tv_nsec = 0 };

    // Wait for user signal within 5 second
    sigset_t sigset;
    siginfo_t info;

    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    
    // for sigwaitinfo() or sigtimedwait().
    // Before test it, you have to block these specified signal.
    // Otherwise will result a undefined behavior.
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    printf("My pid = %ld, send me USR signal.\n", (long)getpid());

    // Wait for SIGUSR1 for 5 sec
    int respond = sigtimedwait(&sigset, &info, &ts);
    
    if(respond != SIGUSR1)
        printf("Time up.\n");
    else
        printf("You made it.\n");

    return 0;
}
