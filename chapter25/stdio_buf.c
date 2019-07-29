/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             stdio_buf.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Demonstrating flush problem with child process
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

// ./a.out 
// Hello World
// Ciao
//
// ./a.out | cat
// Caio
// Hello World
// Hello World
// If target output is not a terminal, the buffer for stdio will be full buffered
// And fork() will also copy the stdio buffer, so both process flush the content during termination.
// to fix this 
//
// 1. flush() before fork.
// 2. use setvbuf() to close the buffering.
// 3, call _exit(), so process won't flush buffer during termination.

int main(int argc, const char *argv[]){
    printf("Hello World\n");
    write(STDOUT_FILENO, "Ciao\n", 5);

    if(fork() == -1)
        errExit("fork");


    exit(EXIT_SUCCESS);
}
