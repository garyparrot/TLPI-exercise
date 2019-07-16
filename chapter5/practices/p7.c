/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p7.c
*
* Author:           garyparrot
* Created:          2019/07/17
* Description:      Chapter 5 problem 7
******************************************************************************/

// First Write data to the specified file
// Then Read it back
// Check if the result is equal

#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h> /* SSIZE_MAX */
#include "tlpi_hdr.h"

// btw, the difference between size_t and ssize_t is unsigned and signed.

struct iovec {
    void *iov_base;
    size_t iov_len;
};

ssize_t writev(int fd, struct iovec *iov, int iovcnt){

    if(iovcnt < 0)
        return errno = EINVAL, -1;

    // check the sum of vector bytes doesn't overflow ssize_t
    size_t sum = 0;
    for(int i = 0;i < iovcnt; i++)
        if((sum += iov[i].iov_len) > SSIZE_MAX)
            return errno = EINVAL, -1;

    ssize_t tot = 0, w;
    for(int i = 0 ;i < iovcnt; i++)
        if(( w = write(fd, iov[i].iov_base, iov[i].iov_len)) >= 0)
        {
            tot += w;
            if(w != iov[i].iov_len) return tot;
        }
        else
            return tot;

    return tot;
}

ssize_t readv(int fd, struct iovec *iov, int iovcnt){

    if(iovcnt < 0)
        return errno = EINVAL, -1;

    // check the sum of vector bytes doesn't overflow ssize_t
    size_t sum = 0;
    for(int i = 0;i < iovcnt; i++)
        if((sum += iov[i].iov_len) > SSIZE_MAX)
            return errno = EINVAL, -1;

    ssize_t tot = 0, r;
    for(int i = 0;i < iovcnt; i++)
        if((r = read(fd, iov[i].iov_base, iov[i].iov_len)) >= 0)
        {
            tot += r;
            if(r != iov[i].iov_len) return tot;
        }
        else
            return tot;

    return tot;
}

int main(int argc, const char *argv[]){

    int fd0 = open("file", O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
    if(fd0 == -1) errExit("open");

#define buffers 100
#define content_size 4096
    struct iovec *vec = malloc(sizeof(struct iovec) * buffers);
    char* buf[buffers];

    // allocate arbitrary data for writing
    for(int i = 0;i < buffers;i++)
    {
        vec[i].iov_base = buf[i] = malloc(sizeof(char) * content_size);
        vec[i].iov_len = content_size;
        for(int j = 0;j < content_size;j++)
            buf[i][j] = ('A' + j % 26);
    }

    writev(fd0, vec, buffers);

    char* read_buf[buffers];
    for(int i = 0;i < buffers;i++){
        vec[i].iov_base = read_buf[i] = malloc(sizeof(char) * content_size);
        vec[i].iov_len = content_size;
    }

    if(lseek(fd0, 0, SEEK_SET) == -1) errExit("lseek");

    readv(fd0, vec, buffers);

    for(int i = 0;i < buffers;i++)
        if(memcmp(buf[i], read_buf[i], content_size) != 0)
            fatal("That's not right.");
        
    puts("Good");
    return 0;
}
