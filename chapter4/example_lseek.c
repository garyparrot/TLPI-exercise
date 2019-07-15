// hello
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

/* Every file our program opened, the kernel will maintain some information about that file.
 * kernel maintain the offset we read, with lseek system call. we can change the offset value.
 * lseek only change the offset value which kernel maintaining, it won't cause any physical access.
 *
 * Note that you cannot change the offset for terminal input / socket / pipe / device, it will 
 * work for files.
 *
 * 
 */

int main(int argc, char *argv[])
{
    int fd;
    if((fd = open("example_lseek.c", O_RDONLY)) == -1) errExit("open");

    char buffer[1024];

    if(lseek(fd, 3, SEEK_SET) == -1) errExit("lseek");
    if(read(fd, buffer, 5) == -1) errExit("read");
    if(write(STDIN_FILENO, buffer, 5) != 5) errExit("write");

    if(lseek(fd, -7, SEEK_END) == -1) errExit("lseek");
    if(read(fd, buffer, 7) == -1) errExit("read");
    if(write(STDIN_FILENO, buffer, 7) != 7) errExit("write");

    off_t cur;
    if((cur = lseek(fd, 0, SEEK_CUR)) != -1) errExit("lseek");

    printf("Current offset = %ld\n", cur);


    return 0;
}

// world
