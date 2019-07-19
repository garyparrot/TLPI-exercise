/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             id_info.c
*
* Author:           garyparrot
* Created:          2019/07/19
* Description:      show process's id 
******************************************************************************/

#define _GNU_SOURCE /* getresuid getresgid */
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <pwd.h>
#include <grp.h>
#include <sys/fsuid.h>

const char NotFound[] = "???";

const char* uid2name(uid_t uid){
    struct passwd *pwd = getpwuid(uid);
    return (pwd != NULL) ? pwd->pw_name : NotFound;
}
const char* gid2name(gid_t gid){
    struct group *grp = getgrgid(gid);
    return (grp != NULL) ? grp->gr_name : NotFound;
}

int main(int argc, const char *argv[]){
    
    uid_t ruid, euid, suid;
    gid_t rgid, egid, sgid;

    // probably work for linux only.
    if(getresuid(&ruid, &euid, &suid) == -1) errExit("getresuid");
    if(getresgid(&rgid, &egid, &sgid) == -1) errExit("getresgid");

    // get file-system id, linux only
    uid_t fsuid = setfsuid(0);
    gid_t fsgid = setfsgid(0);

    // 
    printf("Real        UID: %s(%ld)\n", uid2name(ruid),  (long)ruid);
    printf("Effective   UID: %s(%ld)\n", uid2name(euid),  (long)euid);
    printf("Saved       UID: %s(%ld)\n", uid2name(suid),  (long)suid);
    printf("File-System UID: %s(%ld)\n", uid2name(fsuid), (long)fsuid);
    puts("\n");

    //
    printf("Real        GID: %s(%ld)\n", gid2name(rgid),  (long)rgid);
    printf("Effective   GID: %s(%ld)\n", gid2name(egid),  (long)egid);
    printf("Saved       GID: %s(%ld)\n", gid2name(sgid),  (long)sgid);
    printf("File-System GID: %s(%ld)\n", gid2name(fsgid), (long)fsgid);
    puts("\n");

    // group
    int size = getgroups(0,NULL);
    gid_t *glist = malloc(sizeof(gid_t) * size);
    getgroups(size, glist);

    printf("Secondary group list: \n");
    for(int i = 0;i < size ; i++)
        printf("   %s(%ld)\n", gid2name(glist[i]), (long)glist[i]);

    return 0;
}
