/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_stat.c
*
* Author:           garyparrot
* Created:          2019/07/22
* Description:      Chapter 15 Exercise 3, make stat shows time at nano precision
******************************************************************************/

#define _XOPEN_SOURCE 700
#include <sys/stat.h> /* stat lstat fstat */
#include <sys/types.h>
#include <sys/sysmacros.h> /* major() minor() */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "tlpi_hdr.h"

const char* parse_time(struct timespec tmspec){
    static char buf[1024];

    struct tm* time = localtime(&(tmspec.tv_sec));
    // yyyy-mm-dd.mmmuuunnn
    size_t proc = strftime(buf, 1024, "%F", time);
    size_t s = sprintf(buf + proc, ".%ld", tmspec.tv_nsec);
    buf[s+proc] = '\n';
    buf[s+proc+1] = '\0';
    

    return buf;
}

char* gid_to_name(gid_t gid){
    static char buff[64];
    struct group *grp;
    if((grp = getgrgid(gid)) == NULL){
        // Unknow uid, these file might belongs to a user who has been deleted.
        snprintf(buff, 64, "%ld", (long)gid);
        return buff;
    }

    snprintf(buff, 64, "%s(%ld)", grp->gr_name, (long)gid);
    return buff;
}
char* uid_to_name(uid_t uid){
    static char buff[64];
    struct passwd *pwd;
    if((pwd = getpwuid(uid)) == NULL){
        // Unknow uid, these file might belongs to a user who has been deleted.
        snprintf(buff,64,"%ld", (long)uid);
        return buff;
    }

    snprintf(buff,64,"%s(%ld)", pwd->pw_name, (long)uid);
    return buff;
}

const char* file_type(mode_t mode){
    // this function return the type of file. 
    // the actual value of specific file is defined by system, each system might present
    // each type by different way.
    // better test it with marco
    if(S_ISREG(mode)) return "regular file";
    if(S_ISDIR(mode)) return "directory";
    if(S_ISCHR(mode)) return "character device";
    if(S_ISBLK(mode)) return "block device";
    if(S_ISFIFO(mode)) return "FIFO";
    if(S_ISSOCK(mode)) return "socket";
    if(S_ISLNK(mode)) return "symbolic link";
    return "unknown file";
}

int main(int argc, char* const *argv){
    
    int follow_link = 0;

    for(int opt; (opt = getopt(argc, argv, ":f")) != -1; ){
        switch(opt){
            case 'f': follow_link = 1; break;
            case ':': usageErr("%s [-f] filename", argv[0]); break;
            case '?': usageErr("%s [-f] filename", argv[0]); break;
            default : usageErr("%s [-f] filename", argv[0]); break;
        }
    }
    if(optind != argc - 1) usageErr("%s [-f] filename", argv[0]); 
    if(errno != 0) errExit("");

    const char *filename = argv[optind];
    // stat() will extract symbolic link while lstat() don't.
    // fstat() accept a file descriptor instead of filename.
    //
    int (*get_stat)(const char*, struct stat*) = (follow_link ? stat : lstat);

    struct stat statbuf;

    // if the function success, statfuff will contain any attributes about the file
    if(get_stat(filename, &statbuf) == -1) 
        errExit(follow_link ? "stat" : "lstat");


    /* get absolute path to file */
    printf("File info: %s\n", follow_link ? realpath(filename, NULL) : filename);
    // given a i-node value and device number, you can locate a file on a system.
    // major and minor is a marco, used to extract main-device-id, and sub-device-id 
    printf("    " "dev          : %ld-%ld\n", (long)major(statbuf.st_dev), (long)minor(statbuf.st_dev));
    printf("    " "i-node       : %ld\n", (long)(statbuf.st_ino  ));
    printf("    " "file type    : %s\n", file_type(statbuf.st_mode));
    printf("    " "permission   : %c%c%c%c%c%c%c%c%c\n",
                                  statbuf.st_mode & S_IRUSR ? 'r' : '-',
                                  statbuf.st_mode & S_IWUSR ? 'w' : '-',
                                  statbuf.st_mode & S_IXUSR ? 'x' : '-',
                                  statbuf.st_mode & S_IRGRP ? 'r' : '-',
                                  statbuf.st_mode & S_IWGRP ? 'w' : '-',
                                  statbuf.st_mode & S_IXGRP ? 'x' : '-',
                                  statbuf.st_mode & S_IROTH ? 'r' : '-',
                                  statbuf.st_mode & S_IWOTH ? 'w' : '-',
                                  statbuf.st_mode & S_IXOTH ? 'x' : '-'
                                  );
    printf("%s", statbuf.st_mode & S_ISUID ? "                  Set-UID on\n" : "");
    printf("%s", statbuf.st_mode & S_ISGID ? "                  Set-GID on\n" : "");
    printf("%s", statbuf.st_mode & S_ISVTX ? "                  Sticky  on\n" : "");
    printf("    " "hard refernce: %ld\n", (long)(statbuf.st_nlink));
    printf("    " "Owner        : %s\n", uid_to_name(statbuf.st_uid));
    printf("    " "Group        : %s\n", gid_to_name(statbuf.st_gid));
    printf("    " "Size         : %ld bytes\n", (statbuf.st_size));
    // the unit of block will always be 512 bytes
    // because it is the minimum block on every system that unix have implemented.
    // other system might select a bigger size like 1024, 2048 or 4096 bytes.
    // so the value of st_blocks will always be multiple of 2, 4, 8.
    printf("    " "Block aloced : %ld\n", (long)(statbuf.st_blocks));
    // st_blksize gives the preferred block size for efficient filesystem I/O
    // that means if we want to perform I/O with that file
    // we better select this value as the size of buffer.
    printf("    " "Block optimal: %ld\n", (long)(statbuf.st_blksize));
    printf("    " "Last access  : %s", parse_time(statbuf.st_atim));
    printf("    " "Last modified: %s", parse_time(statbuf.st_mtim));
    printf("    " "Last stat chg: %s", parse_time(statbuf.st_ctim));
    // printf("    " "Last access  : %s", ctime(&(statbuf.st_atime)));
    // printf("    " "Last modified: %s", ctime(&(statbuf.st_mtime)));
    // printf("    " "Last stat chg: %s", ctime(&(statbuf.st_ctime)));

    return 0;
}
