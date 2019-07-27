/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_sigqueue.c
*
* Author:           garyparrot
* Created:          2019/07/27
* Description:      Signal
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){


    if(argc < 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pid sig-num data [num-sigs]", argv[0]);

    printf("%s: PID is %ld, UID is %ld\n", argv[0], (long)getpid(), (long)getuid());

    int sig = get_int(argv[2], GN_NOFLAG, GN_ANY_BASE  );
    int sigData = get_int(argv[3], GN_NOFLAG, GN_ANY_BASE);
    int num = (argc > 4) ? get_int(argv[4], GN_NUM_POSTIVE | GN_NUM_NOT_ZERO, GN_ANY_BASE) : 1;
    
    for(int i = 0;i < num; i++){
        union sigval sv;
        sv.sival_int = sigData + i;
        if(sigqueue(get_long(argv[1], GN_NOFLAG, GN_BASE10), sig, sv) == -1)
            errExit("Sigqueue %d\n", i);
    }

    return 0;
}
