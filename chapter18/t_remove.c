/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_remove.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      Demonstrating Remove
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

    if(argc != 2) usageErr("%s file", argv[0]);
    
    const char *target = argv[1];

    // remove a file or empty directory, remove won't extract symbolic link
    // if file is a symbolic link, call unlink() for the symbolic link
    // if file is a regular file, call unlink()
    // if file is a directory, call rmdir()
    if(remove(target) == -1) errExit("remove");

    return 0;
}
