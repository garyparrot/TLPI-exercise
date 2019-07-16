/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_readv.c
*
* Author:           garyparrot
* Created:          2019/07/16
* Description:      writev: write data into multiple buffers
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>    /* definitions for vector I/O operations */
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    const char* db_path = argv[1];
    int fd;
    if((fd = open(db_path, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR)) == -1) errExit("open");

    char username[10]; int age;

    struct iovec iov[2] = {
        { .iov_base = &username[0] , .iov_len = sizeof(username) },
        { .iov_base = &age, .iov_len = sizeof(age) }
    };
    ssize_t required = sizeof(username) + sizeof(age);

    printf("Enter username(at most 9 character) and age(postive and less than 200): \n");

    ssize_t wrote;
    // buffer overflow, whatever.
    while(scanf("%s%d", username, &age) == 2){
        if((wrote = writev(fd, iov, 2)) == -1)
            errExit("writev");
        if(wrote != required)
            errExit("write fewer bytes than requested");
    }
    
    return 0;
}
