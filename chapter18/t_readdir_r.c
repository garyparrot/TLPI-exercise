/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_readdir_r.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      Demonstrating readdir_r()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    // reentrant version of readdir()

    const char *path = argv[1];

    DIR* dirp;
    struct dirent dirinfo, *dir;

    if((dirp = opendir(path)) == NULL)
        errExit("opendir");

    while(readdir_r(&dirp,))
    
    
    return 0;
}
