/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             orphaned_pgrp_SIGHUP.c
*
* Author:           garyparrot
* Created:          2019/09/07
* Description:      Demonstrate how unix deal with orphane group.
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
    printf("PID=%ld: caught signal %d (%s)\n", (long)getpid(), sig, strsignal(sig));
}

int main(int argc, const char *argv[]){

    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s {s|p} ...\n", argv[0]);

    setbuf(stdout, NULL);

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if(sigaction(SIGHUP , &sa, NULL) == -1)
        errExit("sigaction");
    if(sigaction(SIGCONT, &sa, NULL) == -1)
        errExit("sigaction");

    printf("parent: PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n",
            (long) getpid() , (long) getppid(),
            (long) getpgrp(), (long) getsid(0));

    for(int j = 1;j < argc; j++){
        switch(fork()){
            case -1:
                errExit("fork");
            case 0:
                printf("child: PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n",
                        (long) getpid(), (long) getppid(),
                        (long)getpgrp(), (long) getsid(0));
                if(argv[j][0] == 's'){
                    printf("PID=%ld stopping\n", (long)getpid());
                    raise(SIGSTOP);
                } else {
                    alarm(60);
                    printf("PID=%ld pausing\n", (long)getpid());
                    pause();
                }
                _exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    sleep(3);
    printf("parent exiting\n");
    exit(EXIT_SUCCESS);
    
    return 0;
}
