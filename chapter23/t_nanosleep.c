/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_nanosleep.c
*
* Author:           garyparrot
* Created:          2019/07/28
* Description:      Demonstrating nanosleep
******************************************************************************/

// For sleep(), some implement used software timer. That means they might share
// the same timer with alarm() or setitimer(). For more portability, avoid this..

// Nanosleep provide higher resolution for sleep
// unlink sleep(), SUSv3 require nanosleep() cannot implement via signal.
// That means this function can be used with alarm() or setitimer() in the same time.

// If nanosleep was iterrupt by signal, it will set errno to EINTR and the remaining 
// time will be place and 3rd arguement.

// Even though this is nanosleep, this function still restrict by the software clock 
// limit. ( if kernel support high-res-timer, nanosleep is actually nano level sleep)

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

    struct timeval st, dt;

    if(gettimeofday(&st, NULL) == -1) errExit("gettimeofday");

    struct timespec request, remain;

    request.tv_sec = get_long(argv[1], GN_NOFLAG, GN_ANY_BASE);
    request.tv_nsec= get_long(argv[2], GN_NOFLAG, GN_ANY_BASE);

    for(;;){
        int s = nanosleep(&request, &remain);
        if(s == -1 && errno != EINTR) errExit("nanosleep");

        if(gettimeofday(&dt, NULL) == -1) errExit("gettimeofday");

        printf("Slept for: %9.6f secs\n", dt.tv_sec - st.tv_sec + (dt.tv_usec - st.tv_usec) / 1000000.0 );

        if(s == 0) break;

        printf("Remaining: %2ld.%09ld\n", (long) remain.tv_sec, remain.tv_nsec);

        request = remain;

    }
    
    return 0;
}
