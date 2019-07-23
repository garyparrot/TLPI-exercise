/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/23
* Description:      Chapter 17 Exercise 1
******************************************************************************/

// Who design this anti-human API ?

#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <acl/libacl.h>
#include <sys/acl.h>

#include "tlpi_hdr.h"

// No one understand what the exercise is talking about.
// I guess it means input a ACL file and a user or group.
// show the user or group's permission ?

int main(int argc, const char *argv[]){

    int po_user = 0, po_group = 0, p_user = 0, p_group = 0, p_other = 0, p_mask= 0;
    acl_t acl;

    if(argc != 4) usageErr("%s pathname (u|g) name", argv[0]);
    
    const char* pathname  = argv[1];
    const char* stat_type = argv[2];
    const char* username  = argv[3];
    long myuid = -1; long mygid = -1;

    // get my uid or my gid 
    if(stat_type[0] == 'u'){
        struct passwd *pwd = getpwnam(username);
        if(pwd == NULL) fatal("No such user");
        myuid = (long)(pwd->pw_uid);
    }else if(stat_type[0] == 'g'){
        struct group *grp = getgrnam(username);
        if(grp == NULL) fatal("No such group");
        mygid = (long)(grp->gr_gid);
    }else{
        fatal("Wrong user/group selector %s, it should be 'u' or 'g'.", stat_type);
    }

    // file info
    struct stat fstatus;
    if(stat(pathname, &fstatus) == -1) errExit("stat");

    // get file ACL info
    if((acl = acl_get_file(pathname, ACL_TYPE_ACCESS)) == NULL)
        errExit("acl_get_file");

    // Iterate each ACL entry
    for(int nxt = ACL_FIRST_ENTRY ; ; nxt = ACL_NEXT_ENTRY){
        acl_entry_t entry;

        /* next entry, if 0 is returned, exit */
        if(acl_get_entry(acl, nxt, &entry) != 1)
            break;

        // Type of entry
        acl_tag_t tag;
        if(acl_get_tag_type(entry, &tag) == -1) errExit("acl_get_tag_type");

#define permset_to_mode_t(permset)  (acl_get_perm(permset, ACL_READ) << 2) | \
                                    (acl_get_perm(permset, ACL_WRITE) << 1) | \
                                    (acl_get_perm(permset, ACL_EXECUTE))
#define get_permset(entry, perm)  if(acl_get_permset(entry, perm) == -1) errExit("acl_get_permset");
        acl_permset_t perm;
        uid_t *uid;
        gid_t *gid;
        switch(tag){
            case ACL_USER_OBJ:
                if(myuid != 0) if(fstatus.st_uid != myuid) break;
                get_permset(entry, &perm);
                po_user = permset_to_mode_t(perm);
                break;
            case ACL_GROUP_OBJ:
                if(myuid != 0) if(fstatus.st_gid != mygid) break;
                get_permset(entry, &perm);
                po_group = permset_to_mode_t(perm);
                break;
            case ACL_USER:
                uid = acl_get_qualifier(entry);
                if(myuid != 0) if((long)(*uid) != myuid) break;
                get_permset(entry, &perm);
                p_user = permset_to_mode_t(perm);
                break;
            case ACL_GROUP:
                gid = acl_get_qualifier(entry);
                if(myuid != 0) if((long)(*gid) != mygid) break;
                get_permset(entry, &perm);
                p_group = permset_to_mode_t(perm);
                break;
            case ACL_MASK:
                get_permset(entry, &perm);
                p_mask = permset_to_mode_t(perm);
                break;
            case ACL_OTHER:
                get_permset(entry, &perm);
                p_other = permset_to_mode_t(perm);
                break;
            deafult:
                fatal("Unknow flag %ld", (long)tag);
        }
    }

    // printf("%o %o %o %o %o mask: %o\n", po_user, po_group, p_user, p_group, p_other, p_mask);
    /* Mask on, fuck it. Mask off. */
    /* Mask on, fuck it. Mask off. */
    po_group &= p_mask;
    p_group  &= p_mask;
    p_user   &= p_mask;


    /* erase useless permission set, make them 0 */
    if(myuid == -1 && myuid != 0) { po_user = 0; p_user = 0; }
    if(mygid == -1 && myuid != 0) { po_group = 0; p_group = 0; }

    /* We got all the info, time to determine the privilege of user */
    int has_write_access   = (po_user & 0x4) || (po_group & 0x4) || (p_user & 0x4) || (p_group & 0x4) || (p_other & 0x04);
    int has_execute_access = (po_user & 0x2) || (po_group & 0x2) || (p_user & 0x2) || (p_group & 0x2) || (p_other & 0x02);
    int has_read_access    = (po_user & 0x1) || (po_group & 0x1) || (p_user & 0x1) || (p_group & 0x1) || (p_other & 0x01);

#define rrr(s) (s ? "r" : "-")
#define www(s) (s ? "w" : "-")
#define xxx(s) (s ? "x" : "-")
#define all_mix (po_group | po_user | p_group | p_user | p_other)
    long idid = (myuid == -1) ? mygid : myuid;
    if(myuid == 0){
        printf("%s(%ld) %s%s%s\n", username, (long)idid, rrr(has_read_access), www(has_write_access), xxx(has_execute_access));
    }else{
        printf("%s(%ld) %s%s%s\n", username, (long)idid, rrr(all_mix & 0x04), www(all_mix & 0x02), xxx(all_mix & 0x01));
    }

    
    return 0;
}
