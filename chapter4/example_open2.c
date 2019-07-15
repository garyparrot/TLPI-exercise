#define _GNU_SOURCE
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int fd;
    /* If the file is creat by the open function.
     * The permission of that file will be specified by the 3rd argument.
     *
     * If that argument is not provided. 
     * Permission will be the random value from the runtime stack.
     *
     *
     * Check the implementation of glibc for more detail.
     * https://code.woboq.org/userspace/glibc/sysdeps/unix/sysv/linux/open.c.html
     * */
    if(( fd = open("the_file", O_RDWR | O_CREAT, 0777))==-1) errExit(__FILE__ ":%d open", __LINE__);
    if(close(fd) == -1) errExit(__FILE__ ":%d close", __LINE__);

    /* 
     * Don't update atime(Access time).
     * This flag require the user own the specified file or the process has CAP_FOWNER capability.
     * Otherwise EPERM error is set.
     *
     * This flag is enable by _GNU_SOURCE marco
     */
    if(( fd = open("the_file", O_RDWR | O_NOATIME, 0777))==-1) errExit(__FILE__ ":%d open", __LINE__);
    if(close(fd) == -1) errExit(__FILE__ ":%d close", __LINE__);

    /* Don't extract symbolic link.
     * If the path to file contain symbolic link, a ELOOP error will set.
     */
    if(( fd = open("the_file", O_RDWR | O_NOFOLLOW, 0777))==-1) errExit(__FILE__ ":%d open", __LINE__);
    if(close(fd) == -1) errExit(__FILE__ ":%d close", __LINE__);


    return 0;
}
