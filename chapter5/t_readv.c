/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_readv.c
*
* Author:           garyparrot
* Created:          2019/07/16
* Description:      readv: read data into multiple buffers
******************************************************************************/

// advantage of vector I/O
// 1. Atomic operation: better than a series of read/write, it's atomic.
// 2. Fast: vector I/O only cost you one system call, multiple read/write might be slower.

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>    /* definitions for vector I/O operations */
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    const char* db_path = argv[1];
    int fd;
    if((fd = open(db_path, O_RDONLY)) == -1) errExit("open");

    char username[10]; int age;

    struct iovec iov[2] = {
        { .iov_base = &username[0] , .iov_len = sizeof(username) },
        { .iov_base = &age, .iov_len = sizeof(age) }
    };
    ssize_t require = sizeof(username) + sizeof(age);
    ssize_t readed = 0;

    // print all the entries.
    for(int i = 0; (readed = readv(fd, iov, 2)) > 0; i++)
    {
        if(readed < require) errExit("Read fewer bytes than requested");
        printf("[Entry %2d] username: %s, age: %d\n", i, username, age);
    }

    if(errno) errExit("readv");
    
    return 0;
}
