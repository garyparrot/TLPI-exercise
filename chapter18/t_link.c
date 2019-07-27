/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_link.c
*
* Author:           garyparrot
* Created:          2019/07/23
* Description:      Demonstrating link()
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

    if(argc != 3) usageErr("%s oldpath newpath", argv[0]);

    const char *oldpath = argv[1];
    const char *newpath = argv[2];

    /* link creat newpath as a link to oldpath
     * 1. If oldpath is just a file , newpath will be a hard link to oldfile
     * 2. If oldpath is a symbolic link point to c, newpath will be a symbolic link points c too ( create newpath as symbolic link )
     * As the result, link might create hard link or symbolic link depend on the type of oldpath file is.
     *
     * According to SUSv3, link should always extract symbolic link but link() will be a exception.
     * Linux's link() behavior conflict to SUSv3, most of UNIX follow SUSv3's rule.
     *
     * Avoid link() to a symbolic link for better portability.
     * */
    if(link(oldpath, newpath) == -1)
        errExit("link");
    
    return 0;
}
