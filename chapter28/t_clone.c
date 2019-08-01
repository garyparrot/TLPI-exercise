/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_clone.c
*
* Author:           garyparrot
* Created:          2019/08/01
* Description:      Demonstrate clone()
******************************************************************************/

#define _GNU_SOURCE /* clone() */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sched.h>
#include <sys/wait.h>

#include "tlpi_hdr.h"

#ifndef CHILD_SIG
#define CHILD_SIG SIGUSR1
#endif

/* Feature of clone()
 * 1. Share memory page like vfork() 
 * 2. Execute new process in specific function
 * 3. Stack
 * */

static int childFunc(void *arg){
    if(close(*((int*)arg)) == -1)
        errExit("close");

    return 0;
}

int main(int argc, const char *argv[]){

    const int STACK_SIZE = 65536;
    char *stack;
    char *stackTop;
    int s, fd, flags;

    fd = open("/dev/null", O_RDWR);
    if(fd == -1) errExit("open");
    
    flags = (argc > 1) ? CLONE_FILES : 0;

    stack = malloc(STACK_SIZE);
    stackTop = stack + STACK_SIZE;

    if(CHILD_SIG != 0 && CHILD_SIG != SIGCHLD)
        if(signal(CHILD_SIG, SIG_IGN) == SIG_ERR)
            errExit("signal");

    if(clone(childFunc, stackTop, flags | CHILD_SIG, (void*)&fd) == -1)
        errExit("clone");

    if(waitpid(-1, NULL, (CHILD_SIG != SIGCHLD) ? __WCLONE : 0) == -1)
        errExit("waitpid");
    printf("child has terminated\n");

    s = write(fd, "x", 1);
    if(s == -1 && errno == EBADF)
        printf("file descriptor %d has been closed\n", fd);
    else if(s == -1)
        printf("write() on file descriptor %d filed unexpectedly (%s)\n", fd, strerror(errno));
    else
        printf("write() on file descriptor %d succeeded\n", fd);

    return 0;
}
