#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define bit8 "%d%d%d%d%d%d%d%d"
#define bit32 bit8 bit8 bit8 bit8
#define part2(x) (x >> 1) & 0x1, (x) & 0x1
#define part4(x) part2(x >> 2), part2(x)
#define part8(x) part4(x >> 4), part4(x)
#define part16(x) part8(x >> 8), part8(x)
#define part32(x) part16(x >> 16), part16(x)

int main(int argc, char *argv[])
{
    // fctnl: manipulate file descriptor

    int fd;
    if((fd = open(argv[1], O_RDONLY | O_CREAT, 0777)) == -1) errExit("open");

    int flags;
    if((flags = fcntl(fd, F_GETFL)) == -1) errExit("fcntl");

    printf("Current flags " bit32 "\n", part32(flags));
    flags = (flags & ~O_ACCMODE) | O_RDWR;
    printf("Modified flags " bit32 "\n", part32(flags));

    // fcntl could change flag of file after open it. 
    // But we can only change part of flags, not all the flags support this.
    if(fcntl(fd,F_SETFL, flags) == -1) errExit("fcntl");

    return 0;
}
