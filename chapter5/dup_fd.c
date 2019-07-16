#define _GNU_SOURCE
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

// kernel maintain 3 data structure which relate to file descriptor.
// 1. process's file descriptor table
//    maintain descriptor flags, and a pointer to system level fd
// 2. system level file descriptor table
//    maintain offset / state flag / inode pointer
// 3. file system i-node table
//    maintain device type / file lock / ...
//
// When file descriptor been duplicate, the new fd is a process level file 
// descriptor which point to the same file in system level fd table
//
// the duplicated fd could have different flags( O_WRONLY , O_RDONLY, O_RDWR , ... )
// FD_CLOEXEC is always closed for duplicated fd.
//

int main(int argc, const char *argv[]){
    
    int fd0, fd1, fd2;
    
    // Duplicate specified file descriptor, the new fd value will be the smallest available fd number.
    if((fd0 = dup(STDIN_FILENO)) == -1) errExit("dup");
    printf("Copy stdin, new fd = %d\n", fd0);

    // Duplicate specified file descriptor, we can decide which fd number to use. 
    // If that number alrady been used, dup2 will close it automatically.
    if((fd1 = dup2(STDIN_FILENO, 0)) == -1) errExit("dup2");
    printf("Copy stdin, new fd = %d\n", fd1);

    // dup3 just like dup2, but now we can manipulate file descriptor during the duplication.
    // Only support O_CLOEXEC now.
    // require _GNU_SOURCE marco
    if((fd2 = dup3(fd1, 87, O_CLOEXEC)) == -1) errExit("dup3");
    printf("Copy stdin, new fd = %d\n", fd2);
    return 0;
}
