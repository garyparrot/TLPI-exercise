/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p4.c
*
* Author:           garyparrot
* Created:          2019/07/23
* Description:      Chapter 15 Exercise 4 access()
******************************************************************************/

/* I am very SLEEEEEEEEEEEEEEEEEEEPY during the time I coding */
/* So the code might look like shit :p */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

int eaccess(const char* pathname, int mode){
    if(F_OK == mode)
        return 1;

    // file info
    struct stat buf;

    int f = 1;

    if(stat(pathname, &buf) == -1)
    {
        if(errno != ENOENT)
            errExit("stat");
        else
            f = 0;
    }

    // process info
    uid_t euid = geteuid();
    gid_t egid = getegid();

    int r = 0 || buf.st_mode & S_IROTH;
    int w = 0 || buf.st_mode & S_IWOTH;
    int x = 0 || buf.st_mode & S_IXOTH;

    // is privilege user
    if(euid == 0)
        r = 1,
        w = buf.st_mode & S_IWUSR || buf.st_mode & S_IWOTH || buf.st_mode & S_IWGRP,
        x = buf.st_mode & S_IXUSR || buf.st_mode & S_IXOTH || buf.st_mode & S_IXGRP;

    // user permission
    if(euid == buf.st_uid)
        r = r || buf.st_mode & S_IRUSR, w = w || buf.st_mode & S_IWUSR, x = x || buf.st_mode & S_IXUSR;

    // groups info
    int sz = getgroups(0, NULL) + 1;
    gid_t *glist = malloc(sizeof(gid_t) * sz);
    getgroups(sz - 1, glist);
    glist[sz-1] = egid;

    // group permission
    for(int i = 0; i < sz;i++)
        if(egid == glist[i])
            r = r || buf.st_mode & S_IRGRP, w = w || buf.st_mode & S_IWGRP, x = x || buf.st_mode & S_IXGRP; 

    r = (r) || !(mode & R_OK);
    w = (w) || !(mode & W_OK);
    x = (x) || !(mode & X_OK);

    return f && r && w && x;
}


int main(int argc, const char *argv[]){

    if(argc != 3) usageErr("%s file (f|r|w|x)", argv[0]);

    int flag = 0;

    for(int i = 0; i < strlen(argv[2]) ; i++)
        switch(argv[2][i])
        {
            case 'f': flag |= F_OK; break;
            case 'r': flag |= R_OK; break;
            case 'w': flag |= W_OK; break;
            case 'x': flag |= X_OK; break;
        }

    printf("%s\n", eaccess(argv[1], flag) ? "YES" : "NO");
    
    return 0;
}
