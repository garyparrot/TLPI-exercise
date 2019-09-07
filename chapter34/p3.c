/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p3.c
*
* Author:           garyparrot
* Created:          2019/09/07
* Description:      demonstrate that create a new session for group leader is invalid
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

    if(setsid() == -1)
        errExit("setsid");
    
    return 0;
}
