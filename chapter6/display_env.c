/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             display_env.c
*
* Author:           garyparrot
* Created:          2019/07/17
* Description:      Display environment variables.
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

extern char **environ;

int main(int argc, const char *argv[]){
     
    for(char **c = environ; *c != NULL; c++){
        printf("%s\n", *c);
    }    

    return 0;
}
