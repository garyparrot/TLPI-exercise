/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p4.c
*
* Author:           garyparrot
* Created:          2019/09/08
* Description:      demonstrate that exchange data on same cpu is faster than exchange on different cpu
******************************************************************************/

/* 
 * [garyparrot@parrotparty chapter35]$ time ./a.out send 2 | ./a.out receive 2
 * real    0m0.591s
 * user    0m0.077s
 * sys     0m0.515s
 * [garyparrot@parrotparty chapter35]$ time ./a.out send 2 | ./a.out receive 3
 * real    0m0.217s
 * user    0m0.066s
 * sys     0m0.357s
 *
 * IDK why my result looks weird.
 *
 * */
#define _GNU_SOURCE

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){
      
    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s { send | receive } [cpu id]\n", argv[0]);

    int cpu_id = atoi(argv[2]);

    cpu_set_t cset;
    CPU_ZERO(&cset);
    CPU_SET(cpu_id, &cset);

    /* Make sure this processs run on specific CPU */
    if(sched_setaffinity(0, sizeof(cpu_set_t), &cset) == -1)
        errExit("sched_setaffinity");

    /* Send & Receive 400M data */
    char buffer[4096] = {'A'};
    for(int i = 0; i < 102400; i++) {
        if(strcmp(argv[1], "send") == 0){
            if(write(STDOUT_FILENO, buffer, sizeof(buffer)) == -1)
                errExit("write");
        }else if(strcmp(argv[1], "receive") == 0){
            if(read(STDIN_FILENO, buffer, sizeof(buffer)) == -1)
                errExit("read");
        }
    }

    return 0;
}
