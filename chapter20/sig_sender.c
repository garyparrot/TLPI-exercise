/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             sig_sender.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      Sig sender
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
        usageErr("%s pid num-sigs sig-num [sig-num-2]", argv[0]);

    long pid = get_long(argv[1], GN_NOFLAG, GN_ANY_BASE);
    int  num = get_int (argv[2], GN_NOFLAG, GN_ANY_BASE);
    int  sig = get_int (argv[3], GN_NOFLAG, GN_ANY_BASE);

    printf("%s: sending siganal %d to process %ld %d times\n", argv[0], sig, (long)pid, num);

    for(int i = 0;i < num; i++)
        if(kill(pid, sig) == -1) 
            errExit("kill");

    if(argc > 4)
        if(kill(pid, get_int(argv[4], GN_NOFLAG, GN_ANY_BASE)) == -1)
            errExit("kill");
    
    printf("%s : ok\n", argv[0] );
    return 0;
}
