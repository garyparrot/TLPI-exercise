/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             show_sigset.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      Demonstrating sigset_t
******************************************************************************/

// sigset_t is a set of signals.
// SUSv3 didn't restrict how to implement this set
// instead SUSv3 defined a series of function to manipulate sigset_t
//
// Since each platform's implementation might be different.
// Better sticky to these function for manipulate sigset_t struct for better portability.
//
// sigemptyset()
// sigfillset()
// sigaddset()
// sigdelset()
// sigismember()
//
// constant NSIG equal to the biggest signal number plus 1, not part of SUSv3 but most of system support it.
//
// GNU C lib non-standard :
// sigandset()
// sigorset()
// sigisemptyset()
//

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <signal.h>

#include "tlpi_hdr.h"
#include "signal_functions.h"

int main(int argc, const char *argv[]){
    
    printSigMask(stdout, "Blocked");
    printPendingSigs(stdout, "Pending");
    // Any tranfering for blocking signal will be delayed.
    // blocked signal will be pending until signal unblocked

    // sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
    // this function manipulate process's signal mask. ( pthread_sigmask() for thread )
    
    sigset_t previous, blockSet, pendingSet;

    sigemptyset(&blockSet);
    sigaddset(&blockSet, SIGINT);
    sigaddset(&blockSet, SIGQUIT);
    // system won't block SIGKILL and SIGSTOP
    sigprocmask(SIG_BLOCK, &blockSet, &previous);

    printSigMask(stdout, "Current blocked signal: \n");

    // checking pending signal
    for(;;){
        sleep(3);

        // get pending signals
        // Note: sigpending won't tell you how many specific signal is pending.
        sigpending(&pendingSet);

        printf("Pending : \n");
        printSigset(stdout, "\t\t", &pendingSet);
    }

    return 0;
}
