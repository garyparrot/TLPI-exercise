/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             fork_file_sharing.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Demonstating parent and child process share file descriptor
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    int fd, flags;
    char template[] = "/tmp/testXXXXXX";

    setbuf(stdout, NULL);

    fd = mkstemp(template);
    if(fd == -1) errExit("mkstemp");

    printf("File offset before fork(): %lld\n", (long long) lseek(fd, 0, SEEK_CUR));

    if((flags = fcntl(fd, F_GETFL)) == -1)
        errExit("fcntl F_GETFL");

    printf("O_APPEND flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

    switch(fork()){
        case -1:
            errExit("fork");
        case 0:
            if(lseek(fd, 1000, SEEK_CUR) == -1) 
                errExit("lseek %d", __LINE__);

            if((flags = fcntl(fd, F_GETFL)) == -1)
                errExit("fcntl - F_GETFL");

            flags |= O_APPEND;

            if(fcntl(fd, F_SETFL, flags) == -1)
                errExit("fcntl - F_SETFL");

            _exit(EXIT_SUCCESS);

        default:
            if(wait(NULL) == -1) errExit("wait");

            printf("Child was exited\n");
            printf("File offset in parent: %lld\n", (long long) lseek(fd, 0, SEEK_CUR));

            if( (flags = fcntl(fd, F_GETFL)) == -1)
                errExit("wait");

            printf("O_APPEND flags in parent is: %s\n", (flags & O_APPEND) ? "on" : "off");

            exit(EXIT_SUCCESS);
    }
    
    return 0;
}
