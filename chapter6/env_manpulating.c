/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             env_manpulating.c
*
* Author:           garyparrot
* Created:          2019/07/17
* Description:      Manipulate environment variables.
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

// Point to the beginning of environment variable list.
extern char** environ;

void greeting(){
    const char var[] = "USER";
    // getenv return the pointer to the value of specified variable name, return NULL if not found.
    const char *value = getenv(var);

    if(value != NULL)
        printf("Welcome %s\n", value);
    else
        printf("Who the hell are you, stay away.\n");
}

int main(int argc, const char *argv[]){
    
    greeting();

    // change my name
    // putenv() this add the specified string as an new entry in environment table.
    // This string should not allocate on stack since the reference will point
    // at it, put that string somewhere safe.
    if(putenv("USER=0o_PARROT_LORD_o0") != 0) errExit("putenv");

    greeting();

    // setenv() will create a new entry for specified variable name and value.
    // It will allocate a space for the entry automatically. So basically there
    // is no problem to put that argument at stack or anywhere.
    if(setenv("USER","CRAZY_PARROT", 1) != 0) errExit("setenv");

    greeting();

    // clearenv() clear every environment variable for you.
    // you can also assign NULL to environ, this is one way to clean environment variable table
    // this might cause memory leak since some space allocate by setenv didn't get release 
    // during the process.
    if(clearenv() != 0) errExit("clearenv");

    greeting();



    return 0;
}
