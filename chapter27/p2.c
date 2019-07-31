/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/07/31
* Description:      Exercise 2
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <libgen.h>

#include "tlpi_hdr.h"

char **environ;

int execlp(const char *file, const char *arg, ...){
    int len = 0, i;
    va_list va;
    char *c = NULL;

    /* Scanning arguments */
    va_start(va, arg);
    for(const char *c = arg; c != NULL; c = va_arg(va, char*)) len++;

    /* Allocate array */
    const char* (*args) = malloc(sizeof(char*) * (len+1));
    if(args == NULL) goto failure;

    /* Copy arg addr */
    va_end(va);
    va_start(va, arg);
    i = 0;
    for(const char *c = arg; c != NULL; c = va_arg(va, char*))
        args[i++] = c;
    args[i++] = NULL;  /* NULL terminated array */

    /* Retrieve Path */
    char *path = getenv("PATH");

    /* Try to execute the file in current working directory */
    execve(file, (char* const*)args, environ);

    /* Failed? Try all the Paths, note it can't be an absolute path or relative path */
    if(path != NULL && strchr(file,'/') == NULL){
        int filename_len = strlen(file);
        char *s = path;
        while(1){
            char *b = strchr(s, ':');
            if(b == NULL) b = path + strlen(b);
            //   "/base/path/len" + "/" + "filename" + "\0" 
            size_t len = (b - s) + 1 + filename_len + 1; 

            /* Build string */
            c = malloc(sizeof(char) * len);
            if(c == NULL) goto failure;

            c[0] = '\0';
            strncat(c, s, (b - s));
            strcat (c, "/");
            strcat (c, file);

            execve(c, (char* const*)args, environ);

            free(c); c = NULL; s = b + 1;
            if(*b == '\0') break;
        }
    }

failure:
    va_end(va);
    free(c);
    free(args);

    return -1;
}

int main(int argc, char *argv[]){

    /* conversion for (char*)NULL is important */
    execlp(argv[1], basename(argv[1]), "PARTY", "GO", (char*) NULL);
    
    return 0;
}
