/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             passwd.c
*
* Author:           garyparrot
* Created:          2019/07/18
* Description:      getpwnam, getpwuid, retrieve user's passwd info
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <pwd.h> /* getpwnam, getpwuid */

int main(int argc, const char *argv[]){
    // getpwnam() query passwd info by username    
    // getpwuid() query passwd info by uid
    // these function return a pointer to passwd struct, the space is allocated statically during runtime. 
    // Don't use it during multi-threading programming.
    //
    // getpwnam_r(), getpwuid_r() require you to specify a pointer to passwd buffer.

    struct passwd *pwd = getpwnam(getenv("USER"));
    if(pwd != NULL){
        printf("Welcome %s\n", pwd->pw_name);
        printf("Your UID: %d\n", pwd->pw_uid);
        printf("Your GID: %d\n", pwd->pw_gid);
        printf("Info: %s\n", pwd->pw_gecos);
        printf("Home: %s\n", pwd->pw_dir);
        printf("Shell: %s\n", pwd->pw_shell);
    }else if(errno != 0){
        errExit("getpwnam");
    }else{
        printf("Not found %s\n", getenv("USER"));
    }

    return 0;
}
