/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/20
* Description:      Chapter 10 Exercise 1
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <sys/times.h>
#include <time.h>

// A1. (2^32 - tm_utime / sysconf(_SC_CLK_TCK) second
// A2. (2^32 - clock()) / CLOCKS_PER_SEC second
// 
// This demo is useless since my clock_t is defined as a 8 bytes signed integer.
//
//
// > echo "2^32 / 100" | bc -q
// 42949672 second
// > echo "2^32 / 1000000" | bc -q
// 4294 second
//
// Seen clock() overflow faster than times()
// About 1 hour.

int main(int argc, const char *argv[]){

    clock_t max_clock = 0; max_clock -= 1;
    long timeTick = sysconf(_SC_CLK_TCK);

    struct tms tmm;
    clock_t ck;

    printf("Size of clock_t = %ld\n", sizeof(clock_t));

    while(true){
        times(&tmm);
        ck = clock();

        printf("Current times() = %ld, after %.2lf sec overflow.\n", (long)tmm.tms_utime, (double)(max_clock - tmm.tms_utime + 1) / timeTick);
        printf("Current clock() = %ld, after %.2lf sec overflow.\n", (long)ck, (double)(max_clock - ck + 1) / CLOCKS_PER_SEC);

        usleep(100000);
    }
    
    return 0;
}
