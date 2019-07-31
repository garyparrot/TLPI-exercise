/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_execle.c
*
* Author:           garyparrot
* Created:          2019/07/30
* Description:      Demonstrate t_execle()
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

    if(argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    char *envVec[] = { "GREET=salut", "BYE=adieu" , NULL };
    char *filename = strrchr(argv[1], '/');
    if(filename != NULL)
        filename++;
    else
        filename = argv[1];

    execle(argv[1], filename, "HELLOWORLD", (char *)NULL, envVec);
    errExit("execle");

    return 0;
}
