/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             passwd_entry.c
*
* Author:           garyparrot
* Created:          2019/07/18
* Description:      getpwent, setpwent, endpwent. scan passwd entry
******************************************************************************/

#include <pwd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    // Perform iteration on /etc/passwd file.
    // Also there is setgrent/getgrent/endgrent

    setpwent();

    struct passwd *entry;

    while((entry = getpwent()) != NULL)
        printf("User %s(%d)\n", entry->pw_name, entry->pw_uid);

    endpwent();

    return 0;
}
