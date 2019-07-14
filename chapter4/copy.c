#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, const char *args[]){

    const char* program_name = args[0];
    if(argc != 3 || strcmp(args[1], "--help") == 0)
        usageErr("%s srcfile dstfile", program_name);

    const char* src_filename = args[1];
    int srcfd, dstfd;
    if( (srcfd = open(src_filename, O_RDONLY)) == -1)
        errExit("Open file %s failed.", src_filename);

    const char* dst_filename = args[2];
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int flags = O_CREAT | O_WRONLY | O_TRUNC;
    if( (dstfd = open(dst_filename,flags, mode)) == -1)
        errExit("Open file %s failed.", dst_filename);

    #define BUF_SIZE 1024   /* This value could be change by -D */
    char buf[BUF_SIZE];
    ssize_t readed, wrote ;
    while(( readed = read(srcfd, buf, BUF_SIZE) ) > 0)
        while(readed)
            if((wrote = write(dstfd, buf, readed)) != -1)
                readed -= wrote;
            else
                errExit("Writing file %s failed.", dst_filename);

    if(errno != 0) errExit("Reading file %s failed.", src_filename);
    if(close(srcfd) == -1) errExit("Closing file %s failed.", src_filename);
    if(close(dstfd) == -1) errExit("Closing file %s failed.", dst_filename);
}
