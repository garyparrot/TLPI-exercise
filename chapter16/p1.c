/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/23
* Description:      Chapter 16 Exercise 1. 
*                   Implement a program able to creat or modfiy user EA
******************************************************************************/

#include <sys/xattr.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

void usageError(const char* program_name){ usageErr("%s [-f] pathfile EA_name [ EA_value ]", program_name); }

extern int optind;
extern char* optarg;

int main(int argc, char* const *argv){

    int follow = 0;

    for(int opt; (opt = getopt(argc, argv, ":f")) != -1;){
        switch(opt){
            case 'f': follow = 1; break;
            default: usageError(argv[0]); break;
        }
    }

    if(argc < optind + 3) usageError(argv[0]);
    const char* pathname = argv[optind];
    static char buf[1024]; 
    const char* ea_name = buf; sprintf(buf, "user.%s", argv[optind+1]);
    const char* ea_value= (argc == optind + 3) ? argv[optind+2] : "";
    
    int (*setattr)(const char*, const char*, const void*, size_t, int) = (follow ? setxattr : lsetxattr);

    if(setattr(pathname, ea_name, ea_value, strlen(ea_value), 0) == -1)
        errExit("setattr");
    
    return 0;
}
