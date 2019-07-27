/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_unlink.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      Demonstrating unlink()
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

    // remove file's link to specific inode data, in other words it delete a file name.
    // unlink cannot perform on a directory
    
    if(unlink(argv[1]) == -1)
        errExit("unlink");

    return 0;
}
