/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/07/16
* Description:      Chapter5 practices 2
******************************************************************************/

// Open a file with O_APPEND, change the offset to the beginning of file.
// Then write some data to it, observe the result and explain what's happening.
//
// O_APPEND will set offset to the end of file every time we try to write some data
// so even we move the offset to the beginning of file, the data still goes to the
// tail of file.

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){
    if(argc != 2) usageErr("%s file_name", argv[0]);

    const char* filename = argv[1];    
    int fd;

    // open file with O_APPEND
    if((fd = open(filename, O_RDWR | O_APPEND)) == -1) errExit("open");

    // set the offset to the beginning of file
    if(lseek(fd, 0, SEEK_SET) == -1) errExit("lseek");

    // write data, sizeof() also count the \0 as a character, so we have to 
    // minus the size of writing byte by 1.
    const char data[] = "// Weed party\n";
    if(write(fd, data, sizeof(data)-1) == -1) errExit("write");

    if(close(fd) == -1) errExit("close");

    return 0;
}
// Weed party
// Weed party
// Weed party
