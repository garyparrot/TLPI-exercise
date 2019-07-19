/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             grouplist.c
*
* Author:           garyparrot
* Created:          2019/07/19
* Description:      getgroups
******************************************************************************/

#include "linux/limits.h" /* NGROUPS_MAX */
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <pwd.h>
#include <grp.h>

int main(int argc, const char *argv[]){

    struct passwd *pwd = getpwuid(geteuid());
    if(pwd == NULL) errExit("getpwnam, getenv");

    printf("Username: %s\n",pwd->pw_name);

    int size = getgroups(0,NULL);
    // Note that grouplist contain our effective group id.
    gid_t *grouplist = malloc(sizeof(gid_t) * size);
    getgroups(size, grouplist);

    for(int i = 0 ;i < size; i++){
        struct group *grp = getgrgid(grouplist[i]);
        if(grp != NULL)
            printf("%d. %s(%d)\n", i + 1, grp->gr_name, grp->gr_gid);
        else
            printf("Unknow group gid %d\n", grouplist[i]);
    }
    
    return 0;
}
