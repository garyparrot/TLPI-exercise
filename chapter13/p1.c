#include <fcntl.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"

#define BUF_SIZE 4096

int main(int argc, char *argv[])
{
    if(argc != 3) usageErr("%s src_file dst_file", argv[0]);

    const char* srcfile = argv[1];
    const char* dstfile = argv[2];
    int fd0, fd1;
    if((fd0 = open(srcfile, O_RDONLY)) == -1) errExit("open");
    if((fd1 = open(dstfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP)) == -1) errExit("open");

    char buf[BUF_SIZE];
    for(ssize_t sz; (sz = read(fd0, buf, BUF_SIZE)) > 0; ){
        for(ssize_t wrote = 0, w; wrote < sz; wrote += w) {
            // try to do it quick, four in a row.
            ssize_t skip = 0;
            while(wrote+4 <= sz && *(int*)(&(buf[wrote])) == 0x00000000) wrote+=4, skip += 4;
            while(wrote+1 <= sz && buf[wrote] == '\0') ++wrote, ++skip;

            if(skip > 0)
                if(lseek(fd1, skip, SEEK_CUR) == -1)
                    errExit("lseek");

            if(wrote < sz)
            if((w = write(fd1, buf - wrote, BUF_SIZE - wrote)) == -1)
                errExit("write");
        }
    }
    if(errno != 0) errExit("read");

    if(close(fd0) == -1) errExit("close");
    if(close(fd1) == -1) errExit("close");

    return 0;
}
