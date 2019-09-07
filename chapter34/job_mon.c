/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             job_mon.c
*
* Author:           garyparrot
* Created:          2019/09/07
* Description:      Demonstrating job control
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

static int cmdNum;

static void handler(int sig){
    if(getpid() == getpgrp())
        fprintf(stderr, "Terminal FG process group: %ld\n", (long)tcgetpgrp(STDERR_FILENO));
    fprintf(stderr, "Process %ld (%d) received signal %d (%s)\n", 
            (long)getpid(), cmdNum, sig, strsignal(sig));

    /* SIGTSTP won't actually stop us, we raise SIGSTOP to stop itself */
    /* BTW we can't catch SIGKILL and SIGSTOP */
    if(sig == SIGTSTP)
        raise(SIGSTOP);
}

int main(int argc, const char *argv[]){

    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if(sigaction(SIGINT , &sa, NULL) == -1) errExit("sigaction");
    if(sigaction(SIGTSTP, &sa, NULL) == -1) errExit("sigaction");
    if(sigaction(SIGCONT, &sa, NULL) == -1) errExit("sigaction");

    if(isatty(STDIN_FILENO)){
        fprintf(stderr, "Terminal FG process group: %ld\n", (long)tcgetpgrp(STDIN_FILENO));
        fprintf(stderr, "Command    PID  PPID  PGRP    SID\n");
        cmdNum = 0;
    }else{
        if(read(STDIN_FILENO, &cmdNum, sizeof(cmdNum)) <= 0)
            fatal("read got EOF or error");
    }

    cmdNum++;
    fprintf(stderr, "%4d    %5ld %5ld %5ld\n", cmdNum, 
            (long)getpid(), (long)getppid(), (long)getpgrp(), (long)getsid(0));

    if(!isatty(STDOUT_FILENO))
        if(write(STDOUT_FILENO, &cmdNum, sizeof(cmdNum)) == -1)
            errMsg("write");

    for(;;)
        pause();

    return 0;
}
