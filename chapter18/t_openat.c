/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_openat.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      Demonstrating openat
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

    // openat(int dirfd, const char *pathname, int flags);
    // openat(int dirfd, const char *pathname, int flags, mode_t mode);
    // open specific file as if dirfd is working directory.
    // if pathname is absolute, it doing exact same thing as open
    // if pathname is relative, it will open pathname file as dirfd is the working directory.
    //
    // if dirfd is AT_FDCWD, then openat use process's current working directory.
    // flags let you specify more detail, e.g. AT_SYMLINK_NOFOLLOW prevent extract symbolic link.

    
    return 0;
}
