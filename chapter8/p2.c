/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/07/18
* Description:      Chapter 8 Exercise 2
******************************************************************************/

// Implement getpwnam() by setpwent(), getpwent(), endpwent()

#include <pwd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

struct passwd *getpwnam(const char* name){
    setpwent();

    struct passwd* pwd;

    while((pwd = getpwent()) != NULL)
        if(strcmp(pwd->pw_name, name) == 0)
            return pwd;

    endpwent();

    return NULL;
}

int main(int argc, const char *argv[]){

    printf("%s\n", getpwnam(getenv("USER"))->pw_name);
    printf("%s\n", getpwnam("root")->pw_name);
    printf("%s\n", getpwnam("daemon")->pw_name );
    
    return 0;
}
