/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             acct_on.c
*
* Author:           garyparrot
* Created:          2019/07/31
* Description:      Demonstrate acct()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

// Process Accounting record all the process runs on the device.
// since process accounting might cost a lot of device space
// /proc/sys/kernel/acct limit how much space it can takes.

int main(int argc, char *argv[]){

    if(argc > 2 || (argc > 1 && strcmp(argv[1], "--help") == 0))
        usageErr("%s [file]\n");

    if(strcmp(argv[1], "NULL") == 0)
        argv[1] = NULL;
    
    if(acct(argv[1]) == -1)
        errExit("acct");

    printf("Process accounting %s\n", (argv[1] == NULL) ? "disabled" : "enabled");
    
    return 0;
}
