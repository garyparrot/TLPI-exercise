/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             group.c
*
* Author:           garyparrot
* Created:          2019/07/18
* Description:      getgrnam getgrgid
******************************************************************************/

#include <grp.h>        /* getgrnam getgrgid */
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){
    char buf[1024];
    printf("Group name: "); scanf("%s", buf);
    struct group *grp = getgrnam(buf);

    printf("Group name: %s\n", grp->gr_name);
    printf("Group password: %s\n", grp->gr_passwd);
    printf("GID: %d\n", grp->gr_gid);

    printf("Group member: \n");
    for(int i = 0;grp->gr_mem[i] != NULL;i++)
        printf("    %s\n", grp->gr_mem[i]);

    return 0;
}
