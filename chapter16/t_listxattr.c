/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_listxattr.c
*
* Author:           garyparrot
* Created:          2019/07/23
* Description:      Demonstrating how to to use listxattr()
******************************************************************************/

// EA stands for extended attributes
// file system allow you to add attributes(A pair of name, value string) to file or directory
//
// EA was divide into four categories
// 1. user EA      : any user can set user EA if they have writing permission of specific file.
// 2. trusted EA   : only privilege user or who has CAP_SYS_ADMIN capability can set it.
// 3. system EA    : set by kernel.
// 4. security EA  : store information about security module or executable capability.
//
// for user EA, the name format is defined as user.name

#include <sys/xattr.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

extern int optind;
extern char *optarg;

int main(int argc, char* const *argv){

    int follow_link = 0;

    for(int opt; (opt = getopt(argc, argv, ":f")) != -1 ; ){
        switch(opt){
            case 'f': follow_link = 1; break;
            default : usageErr("%s [-f] pathname", argv[0]);
        }
    }

    ssize_t (*get_attr)(const char*, const char*, void*, size_t) = (follow_link ? getxattr : lgetxattr );
    ssize_t (*list_attr)(const char*, char*, size_t) = (follow_link ? listxattr : llistxattr);
    const char* pathname = argv[1];
    
    ssize_t sz = list_attr( pathname, NULL, 0 );
    if(sz == -1) errExit("list_attr");

    char *buf = malloc(sizeof(char) * sz);
    if(list_attr(pathname, buf, sz) == -1)
        errExit("list_attr");

    for(int i = 0;i < sz;)
    {
        ssize_t lena = get_attr(pathname, buf+i, NULL, 0);
        char *attrbuf = malloc(sizeof(char) * lena);
        get_attr(pathname, buf+i, attrbuf, lena);
        int len = printf("%s: %s\n", buf + i, attrbuf);
        i += strlen(buf+i) + 1;
        free(attrbuf);
    }

    return 0;
}
