/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p5.c
*
* Author:           garyparrot
* Created:          2019/07/16
* Description:      Chapter 5 exercise 5
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <time.h>

Boolean test_valid(int fd0, int fd1){
    off_t off0, off1;
    if((off0 = lseek(fd0, 0, SEEK_CUR)) == -1) errExit("lseek");
    if((off1 = lseek(fd1, 0, SEEK_CUR)) == -1) errExit("lseek");
    return off0 == off1;
}

int main(int argc, const char *argv[]){

    int fd0, fd1;
    if((fd0 = open("testfile", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) errExit("open");
    if((fd1 = dup(fd0)) == -1) errExit("dup");

    srand(time(NULL));

    // read / write / lseek
#define BUF_SIZE 1024
    char buf[BUF_SIZE];
    for(int i = 0;i < 1000; ++i){
        ssize_t ss = rand() % BUF_SIZE;
        switch(rand() % 3){
            case 0:
                if(read(fd0, buf, ss) == -1) errExit("read");
                if(!test_valid(fd0, fd1)) fatal("What the hell is goin on.");
                if(read(fd1, buf, ss) == -1) errExit("read");
                if(!test_valid(fd0, fd1)) fatal("What the hell is goin on.");
                break;
            case 1:
                if(write(fd0, buf, ss) == -1) errExit("write");
                if(!test_valid(fd0, fd1)) fatal("What the hell is goin on.");
                if(write(fd1, buf, ss) == -1) errExit("write");
                if(!test_valid(fd0, fd1)) fatal("What the hell is goin on.");
                break;
            case 2:
                if(lseek(fd0, ss, SEEK_SET) == -1) errExit("write");
                if(!test_valid(fd0, fd1)) fatal("What the hell is goin on.");
                if(lseek(fd1, ss, SEEK_SET) == -1) errExit("write");
                if(!test_valid(fd0, fd1)) fatal("What the hell is goin on.");
                break;
        }
    }

    puts("OK");
        
    return 0;
}
