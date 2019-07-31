/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_execve.c
*
* Author:           garyparrot
* Created:          2019/07/30
* Description:      Demonstrate execve()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

int main(int argc, char *argv[]){

    char *argVec[10];
    char *envVec[] = { "GREET=weed", "BYE=PARTY", NULL };

    if(argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    argVec[0] = strrchr(argv[1], '/');
    if(argVec[0] != NULL)
        argVec[0]++;
    else
        argVec[0] = argv[1];
    argVec[1] = "Hello weed";
    argVec[2] = "Goodbye to the people hated on me.";
    argVec[3] = "Goodbye to the people who loved me.";
    argVec[4] = "Goodbye to the people who trusted me.";
    argVec[5] = "Goodbye goodbye to everybody.";
    argVec[6] = NULL;

    execve(argv[1], argVec, envVec);
    errExit("execve");
    
    return 0;
}
