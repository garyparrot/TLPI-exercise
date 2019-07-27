/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      
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

    mkdir("test", S_IRWXU);
    chdir("test");

    int fd = open("myfile", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    symlink("myfile", "/home/greyparrot/mylink");

    if(errno != 0)
        errExit("?");
    if(chmod("/home/greyparrot/mylink", S_IRUSR) == -1)
        errExit("chmod");
    
    return 0;
}
