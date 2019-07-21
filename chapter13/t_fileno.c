/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_fileno.c
*
* Author:           garyparrot
* Created:          2019/07/21
* Description:      demonstrating fileno and fdopen
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

    // fileno return the fd number of specific FILE struct
    printf("fd of stdin: %d\n", fileno(stdin) );

    // given a fd, fdopen() return a stdio FILE struct
    // this is useful when the specific is a socket or pipe.
    int weed = open("somefile", O_RDWR | O_CREAT | O_TRUNC);
    FILE* f = fdopen(weed, "rw");

    // by the way, write() and stdio() has different route
    // write() will write everything to kerenl buffer cache
    // printf() and other stdio function will put everything to a userspace buffer
    //   and write everything to kerenl buffer cache when condition true
    printf("I am the first!");
    write(fileno(stdout), "I am second line!\n", strlen("I am second line!\n"));
    // the result will be:
    // I am second line!
    // I am the first!
    
    return 0;
}
