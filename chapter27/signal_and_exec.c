/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             signal_and_exec.c
*
* Author:           garyparrot
* Created:          2019/07/31
* Description:      Demonstrating mask&pending signal will preserve even after exec()
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

// During exec(), all the setted disposition(handler) will be reset to SIG_DFL.
// Think about the text segment, it is already gone.
//
// And for these signal who has SIG_DFL or SIG_IGN will stay the same, Expect Ignored SIGCHLD.
// Ignored SIGCHLD will stay the same for Linux, some other implementation of UNIX will set it to SIG_DFL
//
// Also mask and pending won't change

void handler(int sig){ printf("SIGINT"); }

int main(int argc, const char *argv[]){

    printf("First arg : %s\n", argv[0]);

    struct sigaction siga;
    sigset_t sigset;

    if(strcmp(argv[0], "Don't call your self again") != 0)
    {

        /* set sigaction to something else */
        siga.sa_handler = handler;
        siga.sa_flags   = 0;
        sigemptyset(&siga.sa_mask);
        sigaction(SIGINT, &siga , NULL);
        printf("Handler for SIGINT : %s\n", (siga.sa_handler == SIG_DFL) ? "SIG_DFL" :
                                            (siga.sa_handler == SIG_IGN) ? "SIG_IGN" : "Custom Disposition");

        // mask signal
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGINT);
        sigprocmask(SIG_BLOCK, &sigset, NULL);

        // send some signal 
        raise(SIGINT);

        // execute myself
        printf("Execute %s\n\n", argv[0]);
        /* A little hack, change my name to something meanless */
        execl(argv[0], "Don't call your self again", (char*)NULL);
        errExit("execl");

    }

    /* sigaction will be reset after exec */
    sigaction(SIGINT, NULL, &siga);
    printf("Handler for SIGINT : %s\n", (siga.sa_handler == SIG_DFL) ? "SIG_DFL" :
                                        (siga.sa_handler == SIG_IGN) ? "SIG_IGN" : "Custom Disposition");

    /* SIGINT will be setted even after exec() */
    sigemptyset(&sigset);
    sigprocmask(SIG_BLOCK, NULL, &sigset);
    printf("Maksed  SIGINT = %d\n", sigismember(&sigset, SIGINT) ? 1 : 0);
    sigemptyset(&sigset);
    sigpending(&sigset);
    printf("Pending SIGINT = %d\n", sigismember(&sigset, SIGINT) ? 1 : 0);

    sigprocmask(SIG_UNBLOCK, &sigset, NULL);

    return 0;
}
