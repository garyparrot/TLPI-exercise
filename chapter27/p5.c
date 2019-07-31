/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p5.c
*
* Author:           garyparrot
* Created:          2019/07/31
* Description:      Exercise 5
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

// execve won't flush stdio buffer.

int main(int argc, const char *argv[]){
    printf("Hello world");
    execlp("sleep", "sleep", "0", (char*)NULL);
    return 0;
}
