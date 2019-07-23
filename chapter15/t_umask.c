/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_umask.c
*
* Author:           garyparrot
* Created:          2019/07/23
* Description:      Demonstrating umask 
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

    mode_t new_mask = 0777;

    mode_t old_mask = umask(new_mask);

    printf("New mask %o, Old mask %o\n", new_mask, old_mask);
    int fd = open("file", O_RDWR | O_CREAT | O_TRUNC, 0777);
    close(fd);
    
    return 0;
}
