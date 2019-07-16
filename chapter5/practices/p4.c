/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p4.c
*
* Author:           garyparrot
* Created:          2019/07/16
* Description:      Chapter 5 exercise 4
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int fdup(int fd){ return fcntl(fd, F_DUPFD, 0); }
int fdup2(int fd, int sfd) {
    // if the target fd is invalid, return
    if(fcntl(fd, F_GETFD) == -1) return -1;
    // if the target fd is equal to source fd, return 
    if(fd == sfd) return fd;
    // if the target fd is occupied, release it
    if(fcntl(sfd, F_GETFD) != -1){
        errno = 0;
        if(close(sfd) == -1) return -1;
    }

    int copy_fd;
    if((copy_fd = fcntl(fd, F_DUPFD, sfd)) == -1) return -1;

    return copy_fd;
}

int main(int argc, const char *argv[]){
    int a = fdup(STDIN_FILENO); 
    if(a == -1) errExit("fdup");
    if(a !=  3) fatal ("What?");

    int b = fdup2(STDIN_FILENO, 87);
    if(b == -1) errExit("fdup2");
    if(b != 87) fatal  ("what?");

    int c = fdup2(87, 87);
    if(c == -1) errExit("fudp2");
    if(c != 87) fatal  ("WHAT?");

    int d = fdup2(888,0);
    if(d != -1) errExit("what?");

    return 0;
}
