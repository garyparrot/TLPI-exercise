/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             multi_SIGCHLD.c
*
* Author:           garyparrot
* Created:          2019/07/30
* Description:      Demonstrating the right way to handle SIGCHLD.
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include "print_wait_status.h"
#include "curr_time.h"
#include "tlpi_hdr.h"

static volatile int numLiveChildren = 0;

static void sigChldHandler(int sig){
    int status, saveErrno;
    pid_t childPid;

    /* Save errno is is important */
    /* signal handler is a async event, you never know when will be triggered */
    /* If signal happened and change the original errno, things might break. */
    saveErrno = errno;

    printf("%s handler: Caught SIGCHLD\n", currTime("%T"));

    /* Important part: since standard signal won't stack, if there is multiple 
     * signal coming, target process might only receive few of them. */
    while((childPid = waitpid(-1, &status, WNOHANG)) > 0){
        printf("%s handler: Reaped child %ld - ", currTime("%T"), (long)childPid);
        printWaitStatus(NULL, status);
        numLiveChildren--;
    }

    if(childPid == -1 && errno != ECHILD)
        errMsg("waitpid");

    sleep(5);
    printf("%s handler: returning\n", currTime("%T"));

    errno = saveErrno;
}

int main(int argc, const char *argv[]){

    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s child-sleep-time ... \n", argv[0]);

    setbuf(stdout, NULL);
    
    numLiveChildren = argc - 1;

    struct sigaction siga;
    siga.sa_handler= sigChldHandler;
    siga.sa_flags   = 0;
    sigemptyset(&siga.sa_mask);
    if(sigaction(SIGCHLD, &siga, NULL) == -1)
        errExit("sigaction");

    sigset_t blockMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    if(sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1)
        errExit("sigprocmask");

    for(int i = 1; i < argc; i++){
        switch(fork()){
            case -1: errExit("fork");
            case  0:
                sleep(atoi(argv[i]));
                printf("%s Child %d (PId=%ld) exiting\n", currTime("%T"), i, (long)getpid());
                _exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    int sigCnt = 0;
    sigset_t emptyMask;
    sigemptyset(&emptyMask);
    while(numLiveChildren > 0){
        if(sigsuspend(&emptyMask) == -1 && errno != EINTR)
            errExit("sigsuspend");
        sigCnt++;
    }

    printf("%s All %d children have terminated; SIGCHLD was caught %d times\n", currTime("%T"), argc - 1, sigCnt);

    exit(EXIT_SUCCESS);
}
