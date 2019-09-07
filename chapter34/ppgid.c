/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             ppgid.c
*
* Author:           garyparrot
* Created:          2019/09/05
* Description:      Demonstrate getpgrp() and setpgid()
******************************************************************************/

/* process group is a group of processes. */
/* A session is composed with multiple process group, and each session has one control terminal */
/* process group used to manage a task in terminal session. */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

// We also have getpgid(pid) and setpgrp(void), the usage is different with these case

int main(int argc, const char *argv[]){

    // setpgid change the ppid of target process(pid) 
    // There is some cumbersome restriction for it
    // setpgid, if pid is 0, makes pgid change.
    // setpgid, if pgid is 0, makes pgid equal become its pid
    // these code have same result, makes current process belongs to a new process group
    setpgid(0, 0);
    setpgid(getpid(), 0);
    setpgid(getpid(), getpid());
    
    return 0;
}
