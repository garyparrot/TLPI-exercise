/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_fsync.c
*
* Author:           garyparrot
* Created:          2019/07/21
* Description:      Demonstrating fsync, force cache flush to storage device
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
    // Keep record synchronized is a important mission
    // Any system crash might cause file corruption.

    // force specific fd flush its data to storage device
    // fsync forces file state become Synchronized I/O file integrity completion
    // the main point of file integrity is DATA and METADATA
    if(fsync(STDOUT_FILENO) == -1)
        errExit("fsync");

    // similar to fsync(), but hte state is Synchronized I/O data integrity completion
    // the main point of data integrity is DATA, not METADATA
    // call for fdatasync() won't synchronize file metadata. e.g. modification timestamp
    //
    // Usually file data and file metadata are placed on difference section of storage device
    // So this difference could cause a significant speed bonus.
    if(fdatasync(STDOUT_FILENO) == -1)
        errExit("fdatasync")

    // linux implement this function as return after update data to storage system(or at least cache)
    // but SUSv3 allow this function return before its work finished.
    sync();
    
    return 0;
}
