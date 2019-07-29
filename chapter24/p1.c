/******************************************************************************
 *                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Exercise 1
******************************************************************************/

/* fork process and counting value by real time signal */

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

static volatile sig_atomic_t cnt = 0;

void handler(int sig){ cnt += 1; }

int main(int argc, const char *argv[]){

    struct sigaction siga;
    siga.sa_handler = handler;
    siga.sa_flags = SA_RESTART;
    sigemptyset(&siga.sa_mask);

    /* Setup signal */
    sigaction(SIGRTMAX, &siga, NULL);

    int source_process = getpid();

    fork();
    fork();
    fork();

    /* Send to source_process, each process only send once */
    kill(source_process, SIGRTMAX);

    /* if this is source proces. sleep for 0.5sec, then show the result */
    if(getpid() == source_process)
    {
        struct timespec req = { .tv_sec = 0, .tv_nsec = 500000000 }, rem;

        while(1) {
            int ret;
            if((ret = nanosleep(&req, &rem)) != -1 && errno != EINTR)
                errExit("nanosleep");

            if(ret == 0) break;

            req = rem;
        }

        printf("spawned process: %d\n", cnt);
    }
    
    return 0;
}
