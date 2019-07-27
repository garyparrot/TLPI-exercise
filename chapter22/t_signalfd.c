/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_signalfd.c
*
* Author:           garyparrot
* Created:          2019/07/27
* Description:      Demonstrate signalfd()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/signalfd.h>
#include <signal.h>

#include "tlpi_hdr.h"

// signalfd() will create a file descriptor.
// You can read specific signal event form the stream
//
// It's a different to handle signal

int main(int argc, const char *argv[]){

    sigset_t mask;

    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sig-num ...\n", argv[0]);

    printf("%s: PID = %ld\n", argv[0], (long)getpid());

    sigemptyset(&mask);
    for(int i = 1; i < argc; i++)
        sigaddset(&mask, atoi(argv[i]));

    // Mask these signal before signalfd(), otherwise it might not works.
    if(sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
        errExit("sigprocmask");

    int sfd = signalfd(-1, &mask, 0);
    if(sfd == -1) errExit("signalfd");

    for(;;){
        struct signalfd_siginfo fdsi;
        ssize_t s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
        if(s != sizeof(struct signalfd_siginfo))
            errExit("read");

        printf("%s: got signal %d", argv[0], fdsi.ssi_signo);
        if(fdsi.ssi_code == SI_QUEUE)
            printf("; ssi_pid=%d; ssi_int = %d", fdsi.ssi_pid, fdsi.ssi_int);

        printf("\n");
    }
    
    return 0;
}
