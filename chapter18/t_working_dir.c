/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_working_dir.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      Demonstrating how to manipulate process's working directory.
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

    if(argc != 2) usageErr("%s dir", argv[0]);

    const char *path = argv[1];
    char *cwpath = malloc(sizeof(char) * (PATH_MAX + 1)) ;
    
    if(chdir(path) != 0) errExit("chdir");

    // call getcwd(NULL, 0) will allocate a space for you automatically.
    // You should free it manually.
    // Don't relay on this function.
    if(getcwd(cwpath, PATH_MAX+1) == NULL) errExit("getcwd");

    printf("Current working dir : %s\n", cwpath);

    return 0;
}
