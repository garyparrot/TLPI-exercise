/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_symlink.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      Demonstrating symlink() and readlink()
******************************************************************************/

#include <sys/stat.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    if(argc != 3) usageErr("%s filepath link", argv[0]);

    const char *symfile = argv[1];
    const char *target  = argv[2];

    // link can point to a non-exists file or directory
    if(symlink(target, symfile) == -1)
        errExit("symlink");

    // read the link
    char buf[PATH_MAX];
    ssize_t sz = readlink(symfile, buf, PATH_MAX);
    buf[sz] = '\0';

    printf("Point to %s\n", buf);

    return 0;
}
