/*****************************************************************************
* File:             file_hole.c
*
* Author:           garyparrot  
* Created:          07/15/19 
* Description:      Create a file with file hole
*
* ./a.out hole 104857600
* this would creat a file with 100MB file hole.
* If you trying to cat this file, you will find out it cost a lot of time.
* If you trying to evaluate the size of this file, you will find out it only takes a few KB.
*****************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

/* 
 * File hole:
 * You can actually move the offset of file beyond the end of it.
 * and any writing action will actually apply to that file.
 *
 * ---------------------------------------------------------------------------
 * | ..... File content ..... |     File  Hole    | ..... writing data ..... |
 * ---------------------------------------------------------------------------
 *
 * The space which doesn't have any data(that is, writing to that space) called file hole.
 *
 * From a programming perspective, the data in file hole is exists. Any read operation on file hole return null character.
 * But actually these file hole doesn't occupy any space, space allocate once a writing operation happening on it.
 * (Any yes, We have to consider file system allocate a block of bytes.)
 *
 * With file hold, the file size might bigger than its device. 
 */

int main(int argc, char *argv[])
{
    if(argc != 3) usageErr("%s filename hole_size", argv[0]);
    const char *filename = argv[1];
    int hole_size = get_int(argv[2], GN_NOFLAG, GN_ANY_BASE);

    int fd;    
    mode_t mode = S_IRWXU;
    if((fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, mode)) == -1) errExit("open");

    if(lseek(fd, hole_size, SEEK_END) == -1) errExit("lseek");
    const char data[] = "<--- file hold";
    if(write(fd, data, sizeof(data)) != sizeof(data)) errExit("lseek");

    return 0;
}
