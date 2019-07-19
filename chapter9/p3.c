/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p3.c
*
* Author:           garyparrot
* Created:          2019/07/19
* Description:      Chapter 9 exercise 3
******************************************************************************/

// set-user-id is useful, but it only modify your effective ID
// The secondary group ID still be yours(the original real user)

#define _GNU_SOURCE
#define _DEFAULT_SOURCE

#include <linux/limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <pwd.h>
#include <grp.h>

int initgroups(const char* usr , gid_t group){
    // test if we got the permission
    if(geteuid() != 0) return errno = EPERM, -1;
    
    // query user info, for some reason I use getpwnam_r
    struct passwd pwd; struct passwd *pwd_ptr;
    char name_buf[256];
    int err;
    if((err = getpwnam_r(usr, &pwd, name_buf, sizeof(name_buf), &pwd_ptr)) != 0) return errno = err, -1;

    // search for matching group
    static gid_t gt[NGROUPS_MAX + 1];
    int size = 0;
    gt[size++] = group;

    struct group *grp;

    setgrent();

    while((grp = getgrent()) != NULL)
        if(strcmp(grp->gr_name, usr) == 0)
            gt[size++] = grp->gr_gid;

    endgrent();
    
    // set
    if(setgroups(size, gt) == -1) return -1;

    return 0;
}

int main(int argc, const char *argv[]){


    struct passwd *pwd = getpwuid(geteuid());
    printf("change group info to %s\n", pwd->pw_name);

    if(initgroups(pwd->pw_name, getegid()) == -1)
        errExit("initgroups");

    int sz = getgroups(0, NULL);
    gid_t *gs = malloc(sizeof(gid_t) * sz);
    getgroups(sz, gs);

    for(int i = 0;i < sz;i++)
    {
        struct group *grp = getgrgid(gs[i]);
        printf("Secondary group : %s(%ld)\n", grp->gr_name , (long)gs[i]);
    }
    return 0;
}
