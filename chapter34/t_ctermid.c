/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_ctermid.c
*
* Author:           garyparrot
* Created:          2019/09/06
* Description:      Demonstrate ctermid(char*)
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

    /* ctermid return the path to the controlling terminal of current session */
    /* usually the value will be /dev/tty */
    char *term = ctermid(NULL);

    printf("%s\n", term);
    
    return 0;
}
