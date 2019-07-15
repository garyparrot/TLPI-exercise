#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if(argc != 2) usageErr("%s filename", argv[0]);
    if(close(STDIN_FILENO) == -1) errExit("close");
    
    int fd;
    char *filepath = argv[1];
    // SUSv3: The file descriptor open returned always is the smallest available one.
    //        If we close stdin, them the 0 become available. our next open system call 
    //        will be 0.
    if((fd = open(filepath, O_RDONLY)) == -1)
        errExit("open");
    printf("File descriptor number: %d\n", fd);

#define BUF_SIZE 1024

    ssize_t sz;
    char buf[BUF_SIZE];

    /* read system call won't add a null character and the end of input bytes */
    while((sz = read(fd, buf, BUF_SIZE)) > 0)
        /* write system call write value to specified file descriptor
         * 
         * It will return the amount of bytes it wrote.
         * Sometime the written bytes is less than the given bytes, that might caused by device is filled or limitation to file size.
         */
        write(STDOUT_FILENO, buf, sz);
    if(errno != 0)
        errExit("read");

    return 0;
}
