/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_mkdir.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      mkdir 
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    const char *newdir = argv[1];

    // mkdir ignore SUID since it is useless for directory
    // mkdir ignore SGID since it should be set only if parent directory have setted.
    //       in other word, if target parent directory have set SGID, then this new
    //       directory set SGID automatically.
    // Sticky bit work as usual.
    if(mkdir(newdir, S_IRWXU | S_ISGID) == -1)
        errExit("mkdir");

    // rmdir cannot delete non-empty directory.
    if(rmdir(newdir) == -1)
        errExit("rmdir");
    
    return 0;
}
