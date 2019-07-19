/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_systime.c
*
* Author:           garyparrot
* Created:          2019/07/20
* Description:      process running time
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <sys/times.h> /* times */
#include <time.h>      /* clock */

struct tms tmm;

void display(const char *msg){

    static long clockTicks = 0;

    struct tms tmm;
    // times() apply current process's process time.
    if(times(&tmm) == -1) errExit("times");

    clock_t ck = clock();
    // total run time of this process.
    if(ck == (clock_t) -1) errExit("clock");

    if(clockTicks == 0){
        // this is timing unit for times
        clockTicks = sysconf(_SC_CLK_TCK);
        if(clockTicks == -1) 
            errExit("sysconf");
    }

    // even though times() and clock() use same type.
    // their measure unit is quite different.
    // btw times is defined by POSIX.1, and clock is defined by C standard.

    printf("%s\n", msg);

    // CLOCKS_PER_SEC is unit for clock
    printf("clock() = %ld (%.3f sec)\n", ck, (double)ck / CLOCKS_PER_SEC);
    printf("times()   user CPU = %.2f system CPU = %.2f\n", (double)tmm.tms_utime / clockTicks, (double)tmm.tms_stime / clockTicks);
}

int main(int argc, const char *argv[]){

    printf("CLOCKS_PER_SEC=%ld sysconf(_SC_CLK_TCK)=%ld\n\n", (long)CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));

    display("Before");

    int st = (argc > 1) ? get_int(argv[1], GN_NOFLAG, GN_ANY_BASE) : 100000000;
    for(int i = 0;i < st; i++)
        (void) getppid();

    display("After");

    return 0;
}
