/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_chmod.c
*
* Author:           garyparrot
* Created:          2019/07/23
* Description:      Demonstrating chmod
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    // chmod can change permission of specific file
    // It will always extract symbolic link, since symbolic link is always public to everyone
    // There is not way to hide symbolic link for someone on the system.
    if(argc != 3) usageErr("%s filename rwxrwxrwx", argv[0]);
    if(strlen(argv[2]) != 9)  usageErr("%s filename rwxrwxrwx", argv[0]);

    mode_t mode;

    const char fmtter[] = "rwxrwxrwx";
    mode_t mmm[] = {
        S_IRUSR, S_IWUSR, S_IXUSR,
        S_IRGRP, S_IWGRP, S_IXGRP,
        S_IROTH, S_IWOTH, S_IXOTH
    };
        
    for(int i = 0;i < 9 ; i++)
        if(fmtter[i] == argv[2][i])
            mode |= mmm[i];
    
    printf("File %s with %o", argv[1], mode);

    if(chmod(argv[1], mode) == -1)
        errExit("chmod");
    
    return 0;
}
