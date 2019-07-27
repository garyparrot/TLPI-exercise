/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_dirname.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      Demonstrating dirname() and basename()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"
#include <libgen.h> /* dirname() basename() */

int main(int argc, const char *argv[]){

    if(argc != 2) usageErr("%s filename", argv[0]);

    // strdup duplicate string, the new string was allocated by malloc()
    char *t1 = strdup(argv[1]);
    char *t2 = strdup(argv[1]);
    if(t1 == NULL || t2 == NULL)
        errExit("strdup");

    // dirname will modify specified string
    
    printf("%s dir name %s, file name is %s\n", argv[1], dirname(t1), basename(t2));

    free(t1);
    free(t2);
    return 0;
}
