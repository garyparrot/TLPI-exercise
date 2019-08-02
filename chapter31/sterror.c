/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             sterror.c
*
* Author:           garyparrot
* Created:          2019/08/02
* Description:      Non Thread-safe version of strerror() implementation
******************************************************************************/

#define _GNU_SOURCE     /* Get _sys_nerr and _sys_errlist */
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

#define MAX_ERROR_LEN 256

static char buf[MAX_ERROR_LEN];

char *strerror(int err){
    if(err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL)
        snprintf(buf, MAX_ERROR_LEN, "Unknow error %d", err);
    else{
        /* According to man page strcpy(3) */
        /* strncpy copy at most n bytes from source string */
        /* And if the first n bytes are has no null bytes */
        /* The string will not terminated */
        /* To play it safe, we terminate the string no matter what */
        strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
        buf[MAX_ERROR_LEN - 1] = '\0';
    }

    return buf;
}

int main(int argc, const char *argv[]){

    printf("%s\n", strerror(5));
    
    return 0;
}
