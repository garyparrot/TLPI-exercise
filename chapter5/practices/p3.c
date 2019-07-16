/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p3.c
*
* Author:           garyparrot
* Created:          2019/07/16
* Description:      Chapter 5 practices 3
******************************************************************************/

// Result: 
//
// greyparrot@weed ~/P/T/c/practices (master)> ./a.out f2 1000000 x & ./a.out f2 1000000 x
// greyparrot@weed ~/P/T/c/practices (master)> ./a.out f1 1000000 & ./a.out f1 1000000
// greyparrot@weed ~/P/T/c/practices (master)> wc f1
//       0       1 2000000 f1
// greyparrot@weed ~/P/T/c/practices (master)> wc f2
//       0       1 1005364 f2
//
// We run this on a file system that support O_APPEND as a atomic operation.
//
// O_APPEND will guarantee the offset being move to the end of file before writing data.
// when O_APPEND is close and lseek is used instead, the operation is no longer atomic.
// Because when process 1 just move the offset to the end of file, process 2 might write another byte to it.

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    if(argc != 3 && argc != 4) usageErr("%s file_name num-bytes [x]");

    const char* filename = argv[1];
    const long num_bytes = get_long(argv[2], GN_NOFLAG, GN_ANY_BASE);
    const Boolean no_append = (argc >= 4 && strcmp(argv[3], "x") == 0) ? true : false;

    const int file_flag = O_WRONLY | (no_append ? 0 : O_APPEND) | O_CREAT; 
    const mode_t mode = S_IWUSR | S_IRUSR;

    int fd;
    if((fd = open(filename, file_flag, mode)) == -1) errExit("open");

    for(int i = 0;i < num_bytes; i++)
    {
        if(write(fd, "x", 1) != 1)
            errExit("write");
        if(no_append)
            if(lseek(fd, 0 , SEEK_END) == -1)
                errExit("lseek");
    }

    if(close(fd) == -1) errExit("open");

    return 0;
}
