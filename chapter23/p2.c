/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_nanosleep.c
*
* Author:           garyparrot
* Created:          2019/07/28
* Description:      Demonstrating nanosleep
******************************************************************************/

// 180sec No ABSTIME 
// Slept for: 180.000164 secs

// 180sec No ABSTIME, killing process
// Slept for: 183.062703 secs

// 180sec ABSTIME, killing process
// Slept for: 180.000120 secs

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

#include "tlpi_hdr.h"

static void handler(int sig) {  }

int main(int argc, const char *argv[]){

    if(argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s secs nanosecs", argv[0]);

    struct sigaction siga;
    siga.sa_handler = handler;
    siga.sa_flags   = 0;
    sigemptyset(&siga.sa_mask);

    sigaction(SIGINT, &siga, NULL);

    struct timespec st, dt;

    if(clock_gettime(CLOCK_REALTIME, &st)) errExit("clock_gettime");

    struct timespec request, remain;

    request.tv_sec = get_long(argv[1], GN_NOFLAG, GN_ANY_BASE) + st.tv_sec;
    request.tv_nsec= get_long(argv[2], GN_NOFLAG, GN_ANY_BASE) + st.tv_nsec;
    remain.tv_sec = remain.tv_nsec = 0;

    for(;;){
        int s = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &request, &remain);
        if(s != 0 && s != EINTR) errExitEN(s,"clock_nanosleep");

        if(clock_gettime(CLOCK_REALTIME, &dt) == -1) errExit("clock_gettime");

        printf("Slept for: %9.6f secs\n", dt.tv_sec - st.tv_sec + (dt.tv_nsec - st.tv_nsec) / 1000000000.0 );

        if(s == 0) break;
    }
    
    return 0;

}
