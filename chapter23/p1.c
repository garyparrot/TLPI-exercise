/*****
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/28
* Description:      Chapter 23 Exercise 1: Implement alarm() by setitimer()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#include "tlpi_hdr.h"

unsigned int alarm(unsigned int seconds){
    struct itimerval cur, dest;
    if(getitimer(ITIMER_REAL, &cur) == -1) errExit("getitimer");
#define timeval_zero(t) (t.tv_sec == 0 && t.tv_usec == 0)
#define itimerval_zero(t) (timeval_zero(t.it_value) && timeval_zero(t.it_interval))

    /* Cancel timer */
    if(seconds == 0 || !itimerval_zero(cur)){
        memset(&dest, 0, sizeof(dest));  /* fill dest with zero */
        if(setitimer(ITIMER_REAL, &dest, NULL) == -1) errExit("setitimer");
        return cur.it_value.tv_sec;
    }

    /* setup the alarm */
    memset(&dest, 0, sizeof(dest));      /* fill dest with zero */
    dest.it_value.tv_sec = seconds;     

    if(setitimer(ITIMER_REAL, &dest, NULL) == -1) errExit("setitimer");

    return cur.it_value.tv_sec;
}

void handler(int sig) { printf("Receive Signal.\n"); }

int main(int argc, const char *argv[]){

    /* setup signal handler */
    struct sigaction siga;
    siga.sa_handler = handler;
    siga.sa_flags   = 0;
    sigemptyset(&siga.sa_mask);

    if(sigaction(SIGALRM, &siga, NULL) == -1) errExit("sigaction");

    /* setup alarm */
    alarm(1);

    /* make it boom, btw this is unsafe since race condition, but I am lazy to fix it */
    pause();

    /* try to cancel alarm this time */
    struct timespec spec; spec.tv_sec = 2; spec.tv_nsec = 0;
    alarm(3);
    nanosleep(&spec,NULL);        /* since nanosleep is not trigger by signal, it is safe to use it with alarm*/
    alarm(0);
    nanosleep(&spec,NULL);        
    // There shoud be no output.

    return 0;
}
