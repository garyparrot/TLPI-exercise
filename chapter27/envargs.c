/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             envargs.c
*
* Author:           garyparrot
* Created:          2019/07/30
* Description:      Goodbye
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

extern char** environ;

int main(int argc, const char *argv[]){

    for(int i = 0; i < argc ;i++)
        printf("argv[%d] = %s\n", i , argv[i]);

    for(char** ch = environ; *ch != NULL; ch++)
        printf("environ: %s\n", *ch);
    
    return 0;
}
