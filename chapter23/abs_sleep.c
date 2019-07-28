/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             abs_sleep.c
*
* Author:           garyparrot
* Created:          2019/07/28
* Description:      Demonstrate absolute sleep with POSIX clock
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    /* Get resolution of clock */
    struct timespec res;
    if(clock_getres(CLOCK_REALTIME, &res) == -1)
        errExit("clock_getres");

    printf("Resolution of CLOCK_REALTIME : sec=%ld.%09ld\n", res.tv_sec, res.tv_nsec );

    /* Unlike usual timeval, timespec sleep can represent nano second */
    struct timespec request;

    /* Now */
    clock_gettime(CLOCK_REALTIME, &request);

    /* Sleep for 10 sec */
    request.tv_sec += 10;

    /* Sleep with absolute time */
    int s = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &request, NULL);

    if(s != 0){
        if(s == EINTR)
            printf("Interrupted by signal handler.\n");
        else
            errExitEN(s, "clock_nanosleep");
    }

    
    return 0;
}
