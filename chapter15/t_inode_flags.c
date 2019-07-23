/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_inode_flags.c
*
* Author:           garyparrot
* Created:          2019/07/23
* Description:      Demonstrating how to manipulate inode flags
******************************************************************************/

#include <linux/fs.h>   /* FS_IOC_GETFLAGS FS_IOC_SETFLAGS ... */
#include <sys/stat.h>
#include <sys/ioctl.h>  /* ioctl */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    if(argc != 2) usageErr("%s filename",argv[0]);
    
    int fd;

    if( (fd = open(argv[1], O_RDWR)) == -1 )
        errExit("open");

    int attr;

    if(ioctl(fd, FS_IOC_GETFLAGS, &attr) == -1)
        errExit("ioctl");

    /* append APPEND flags for file */
    attr |= FS_APPEND_FL;

    if(ioctl(fd, FS_IOC_SETFLAGS, &attr) == -1)
        errExit("ioctl");

    return 0;
}
