/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p4.c
*
* Author:           garyparrot
* Created:          2019/07/21
* Description:      Exercise 4
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

    // setvbuf(stdout, NULL, _IONBF, 0);

    printf("If I had more time, \n");
    write(STDOUT_FILENO, "I would have written you a shorter letter.\n", 43);
    
    return 0;
}
