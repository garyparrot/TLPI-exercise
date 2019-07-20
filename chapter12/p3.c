/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p3.c
*
* Author:           garyparrot
* Created:          2019/07/20
* Description:      Chapter 12 Exercise 3
******************************************************************************/

#include <pwd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdarg.h>
#include "tlpi_hdr.h"

#define BUF_SIZE 1024

// test if given string is numerical string
Boolean isNumericalStr(const char* str){
    for(const char *c = str; *c != '\0'; c++)
        if(!('0' <= *c && *c <= '9'))
            return false;
    return true;
}

/* parsing specified string */
const char* parsing_dirname(const char* fmt, ...){
    static char path[_POSIX_PATH_MAX+10];

    va_list va;
    va_start(va, fmt);
    vsprintf(path , fmt , va);
    va_end(va);

    // printf("%s\n", path);

    return path;
}

int main(int argc, const char *argv[]){

    if(argc != 2) usageErr("%s [filename]", argv[0]);

    char target_file[BUF_SIZE];
    char link_path[PATH_MAX+10];

    realpath(argv[1], target_file);
    if(errno != 0) errExit("realpath");
    printf("Target file: %s\n", target_file);

    /* Opne /proc directory */
    DIR* proc = opendir("/proc");
    if(errno != 0) errExit("opendir");

    /* loop though each Process file */
    for(struct dirent* dm;(dm = readdir(proc)) != NULL;){
        /* Test if the name is a number */
        if(!isNumericalStr(dm->d_name)) continue;
        
        /* Open file descriptor dir */
        DIR* fd_dir = opendir(parsing_dirname("/proc/%s/fd", dm->d_name));
        if(fd_dir == NULL) { errno = 0; continue; }

        /* loop though each file descriptor */
        for(struct dirent* fds;(fds = readdir(fd_dir)) != NULL;){

            /* readlink() read the value of symbolic link */
            const char* slink = parsing_dirname("/proc/%s/fd/%s", dm->d_name, fds->d_name);
            ssize_t sz = readlink(slink, link_path, sizeof(link_path)-1); link_path[sz] = '\0';

            // printf("%s\n", link_path);

            /* test if it is equal */
            if(strcmp(target_file, link_path) == 0){

                /* get process's status */
                FILE* status = fopen(parsing_dirname("/proc/%s/status", dm->d_name), "r");

                /* Since the process might disappear anytime. If we failed, just ignore it */
                if(status == NULL) {errno = 0; break;}

                pid_t process_id;
                uid_t user_id;
                char program_name[BUF_SIZE];
                char buff[BUF_SIZE];
                /* read status */
                while(fgets(buff, BUF_SIZE, status)){
                    if(strncmp(buff, "Name:", 5) == 0)
                        sscanf(buff+5, " %s", program_name);
                    if(strncmp(buff, "Pid:" , 4) == 0)
                        sscanf(buff+4, " %d", &process_id);
                    if(strncmp(buff, "Uid:" , 4) == 0)
                        sscanf(buff+4, " %d", &user_id);
                }
                /* get userinfo */
                struct passwd *pwd = getpwuid(user_id);
                printf("Process: %s(%ld) user %s(%ld)\n", program_name, (long)process_id, pwd->pw_name, (long)user_id);
            }
            errno = 0;
        }
        errno = 0;
    }
    if(errno != 0) errExit("readdir");

    return 0;
}
