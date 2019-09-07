/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             handling_SIGTSTP.c
*
* Author:           garyparrot
* Created:          2019/09/07
* Description:      Demonstrate how to deal with SIGTSTP
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

/* In previous program we add a handler for SIGTSTP */
/* But this changed the default behavior of SIGTSTP, now we can't stop the program */
/* So we use a hack to work around this - SIGSTOP */
/* But this is not very nice, since now parent process though the process is stopped by SIGSTOP instead of SIGTSTP */
/* The more proper way to handle this is, reset SIGTSTP handler to default, and send it again */
/* after that set the handler back */
/* Now our program is stop by SIGTSTP, the down side is more code :p */

static void tstpHandler(int sig){

    sigset_t tstpMask, prevMask;
    int savedErrno;
    struct sigaction sa;

    savedErrno = errno;

    printf("Caught SIGTSTP\n");

    /* default handler for SIGTSTP */
    if(signal(SIGTSTP, SIG_DFL) == SIG_ERR)
        errExit("signal");

    /* Raise SIGTSTP */
    raise(SIGTSTP);

    sigemptyset(&tstpMask);
    sigaddset(&tstpMask, SIGTSTP);
    /* Unblock SIGTSTP for a while, ensure that signal is handled */
    if(sigprocmask(SIG_UNBLOCK, &tstpMask, &prevMask) == -1)
        errExit("sigprocmask");

    /* Now set it back */
    if(sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
        errExit("sigprocmask");

    /* reset the handler for SIGTSTP */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = tstpHandler;
    if(sigaction(SIGTSTP, &sa, NULL) == -1)
        errExit("sigaction");

    printf("Exiting SIGTSTP handler\n");
    errno = savedErrno;
}

int main(int argc, const char *argv[]){
    struct sigaction sa;

    if(sigaction(SIGTSTP, NULL, &sa) == -1)
        errExit("sigaction");

    if(sa.sa_handler != SIG_IGN){
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = tstpHandler;
        if(sigaction(SIGTSTP, &sa, NULL) == -1)
            errExit("sigaction");
    }

    for(;;){
        pause();
        printf("Main\n");
    }
    return 0;
}
