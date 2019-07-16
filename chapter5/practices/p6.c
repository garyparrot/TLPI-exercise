/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p6.c
*
* Author:           garyparrot
* Created:          2019/07/16
* Description:      Chapter 5 exercise 6
******************************************************************************/

// expect answer : Giddayworld\0

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){
    int fd1, fd2, fd3;
    fd1 = open("file", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    fd2 = dup(fd1);
    fd3 = open("file", O_RDWR);

    write(fd1, "Hello,", 6);
    write(fd2, "world", 6);
    lseek(fd2, 0, SEEK_SET);
    write(fd1, "HELLO,", 6);
    write(fd3, "Gidday", 6);
    return 0;
}
