// HELLO
/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             pread_pwrite.c
*
* Author:           garyparrot
* Created:          2019/07/16
* Description:      pread and pwrite, this program modifiy prefix HELLO to WORLD
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){
    int fd;
    if((fd = open("pread_pwrite.c", O_RDWR)) == -1) errExit("open");

    // pread / pwrite 
    // atomic operation for read/write arbitrary position in file
    // it won't change the fd's offset after function call
    char buf[5];
    if(pread (fd, buf, sizeof(buf), 3) != sizeof(buf)) errExit("pread");
    buf[0] = buf[0] ^ 'H' ^ 'W';
    buf[1] = buf[1] ^ 'E' ^ 'O';
    buf[2] = buf[2] ^ 'L' ^ 'R';
    buf[3] = buf[3] ^ 'L' ^ 'L';
    buf[4] = buf[4] ^ 'O' ^ 'D';
    if(pwrite(fd, buf, sizeof(buf), 3) != sizeof(buf)) errExit("pwrite");

    return 0;
}


