/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             pid.c
*
* Author:           garyparrot
* Created:          2019/07/17
* Description:      PID
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){
    pid_t my_pid = getpid();    
    pid_t parent_pid = getppid();

    // check /proc/sys/kernel/pid_max, pid_max = (the_biggest_pid) + 1
    // There is a Process id counter in kernel, once it reach the limit 
    // of pid_max, kernel will reset the value to 300 and start allocating 
    // from this number.
    //
    // If you saw a program A which has lesser pid compare to B, it doesn't mean
    // A program start before B. 
    printf("My pid=%d, my parent's pid=%d \n", my_pid, parent_pid);

    return 0;
}
