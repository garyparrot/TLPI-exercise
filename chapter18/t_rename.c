/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_rename.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      Demonstrating rename()
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

    if(argc != 3) errExit("%s src dst", argv[0]);

    const char *oldpath = argv[1];
    const char *newpath = argv[2];

    // This call manipulate directory entry, it has nothing to do with the inode content.
    // Theoretical this won't change ctime but in practices it is changed.
    // The reason it get change is because long time ago UNIX implement this feature.
    // And some of the program will failed without this feature, so there it is.
    //
    struct stat buf;
    if(stat(argv[2], &buf) == -1) errExit("stat");
    if((buf.st_mode & S_IFMT) == S_IFDIR) fatal("destination must be a filename, it cannot be a directory");

    // you can rename a directory, but you cannot replace a non-empty dir or replace its child directory.
    
    // newpath and oldpath must located at same file system.
    // Since it is RENAME, the only thing it do is manipulate hard link
    // Not copy or moving file between two filesystem, this cannot be done.

    // if file exists, replace it
    // if oldpath and newpath point to the same file, it won't change.
    // etc rename("x", "abc/../x")
    // rename won't extract symbolic link
    if(rename(oldpath, newpath) == -1)
        errExit("rename");
    
    return 0;
}
