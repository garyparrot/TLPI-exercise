/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p9.c
*
* Author:           garyparrot
* Created:          2019/07/26
* Description:      Chapter 18 Exercise 9
******************************************************************************/

// fchdir will be faster
// 1. fchdir is just a fd, it don't have to eval whole path string like chdir did.
// 2. IDK.

#define _GNU_SOURCE

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    if(argc != 4) usageErr("Usage: %s times (fchdir|chdir) dir2", argv[0]);

    int times = get_int(argv[1], GN_NUM_POSTIVE, GN_ANY_BASE);
    const char* d1 = argv[3];
    char d2[1000];
    (void)getcwd(d2, 1000);

    if(strcmp(argv[2], "fchdir") == 0){
        int fd1 = open(d1, O_RDONLY);
        int fd2 = open(d2, O_RDONLY);
        for(int i = 0;i < times; i++)
            fchdir((i%2) ? fd1 : fd2);
    }else if(strcmp(argv[2], "chdir") == 0){
        for(int i = 0; i < times; i++)
            chdir((i%2) ? d1 : d2);
    }else{
        usageErr("Usage: %s times (fchdir|chdir)", argv[0]);
    }
    
    return 0;
}
