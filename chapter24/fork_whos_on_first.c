/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             fork_whos_on_first.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Who go first
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){
    if(argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [num-children]\n", argv[0]);

    int numChildren = (argc > 1) ? get_int(argv[1], GN_NUM_NOT_ZERO, GN_ANY_BASE) : 1;

    setbuf(stdout, NULL);

    for(int i = 0;i < numChildren; i++){
        int childPid;
        switch (childPid = fork()) {
            case -1:
                errExit("fork");
            case 0:
                printf("%d child\n", i);
                _exit(EXIT_SUCCESS);
            default:
                printf("%d parent\n", i);
                wait(NULL);
                break;
        }
    }
    
    return 0;
}
