#include <fcntl.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int fd;

    // all the system calls are atomic, this could prevent race condition.
    // below code ensure this program is the one who create and open the file.
    if((fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL | O_APPEND, 0600)) == -1)
        errExit("open");

    // Write log to the end of file, We could use O_APPEND flag.
    // With O_APPEND flag, every write system call will move the offset to the end
    // of file automatically.
    const char data[] = "meme";
    if(write(fd, data, sizeof(data)) == -1)
        errExit("write");

    // Note that all these feature is supported by the file system itself.
    // For example, O_APPEND is not support by NFS.
    // When O_APPEND working on a NFS file system, kernel simulate it.
    // And cannot prevent race condition.

    return 0;
}
