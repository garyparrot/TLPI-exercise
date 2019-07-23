/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_utime.c
*
* Author:           garyparrot
* Created:          2019/07/22
* Description:      Demonstrating utime, modify the access time and modified time.
******************************************************************************/

#include <sys/stat.h> /* utimensat */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <utime.h>

#include "tlpi_hdr.h"

// to modify atime and mtime attributes.
// We need CAP_FOWNER or CAP_DAC_OVERRIDE capability, or process's euid match the file's uid and has writing permission.
// (More precisely, the process's fsuid match the file's uid)

int main(int argc, const char *argv[]){

    const char* file = argv[1];

    // like touch, set atime and mtime to this moment.
    utime(file, NULL);

    // set time to the beginning of Epoch 
    struct utimbuf ubuf = {.actime = 0, .modtime = 0};
    utime(file, ubuf);

    // Linux provide utimes(), A system call from BSD. utimes() provide micro second precision.
    
    // futimes() fd version
    // lutimes() just like utimes() but won't extract symbolic link.
    
    // utimensat() provide nano second precision.
    // This function provide very much utility so it is a little bit hard to use.
    // Better check man page.
    struct timespec tsp[2] = { { .tv_sec = 0; .tv_nsec = UTIME_NOW;  }, { .tv_sec = 0; .tv_nsec = UTIME_OMIT; } };
    int fd = open(argv[1], O_WRONLY);
    utimensat( AT_FDCWD, file, tsp, AT_SYMLINK_NOFOLLOW );

    return 0;
}
