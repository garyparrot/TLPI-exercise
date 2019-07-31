/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_fexecve.c
*
* Author:           garyparrot
* Created:          2019/07/30
* Description:      Demonstrate fexecve
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

/* fexecve execute a file by specified file descriptor */
/* It sounds like useless, but actually it is possible that sombody */
/* change the target file before the open operation. And execute another */
/* file. But if the file is opened by file descriptor. */
/* Above method won't works */

int main(int argc, const char *argv[]){
    
    return 0;
}
