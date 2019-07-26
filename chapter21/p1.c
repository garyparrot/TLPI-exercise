/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/26
* Description:      Chapter 21 Exercise 1 
******************************************************************************/

// Implement abort()

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

static void m_abort(void){

    // unblock SIGABRT
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGABRT);
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    // raise SIGABRT
    raise(SIGABRT);

    // If it doesn't abort this process, we will restore the default handler for SIGABRT

    struct sigaction sg;
    sg.sa_handler = SIG_DFL;
    sigemptyset(&sg.sa_mask);
    sg.sa_flags   = 0;
    sigaction(SIGABRT, &sg, NULL);

    // and raise that again
    raise(SIGABRT);
}

int main(int argc, const char *argv[]){

    sigset_t set;
    struct sigaction sg;

    int way = 0;
    if(argc == 2) way = get_int(argv[1], GN_NOFLAG, GN_ANY_BASE);

    if(way & 0x1)
    {
        // mask SIGABRT
        sigemptyset(&set);
        sigaddset(&set, SIGABRT);
        sigprocmask(SIG_BLOCK, &set, NULL);

        printSigMask(stdout, "Current mask:");
    }

    if(way & 0x2)
    {
        // ignore SIGABRT signal
        sg.sa_handler = SIG_IGN;
        sigemptyset(&sg.sa_mask);
        sg.sa_flags = 0;
        sigaction(SIGABRT, &sg, NULL);
    }

    m_abort();

    return 0;
}
