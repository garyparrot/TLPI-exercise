/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/20
* Description:      Chapter 12 Exercise 1
******************************************************************************/

// List user's process.

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <dirent.h>
#include <assert.h>
#include <pwd.h>

#define BUF_SIZE 1024

// transform username into its uid
uid_t get_uid(const char* name){
    struct passwd *pwd = getpwnam(name);
    if(pwd == NULL)
    {
        if(errno != 0)
            errExit("getpwnam");
        else
            fatal("User %s not found.", name);
    }

    return pwd->pw_uid;
}

// test if given string is numerical string
Boolean isNumericalStr(const char* str){
    for(const char *c = str; *c != '\0'; c++)
        if(!('0' <= *c && *c <= '9'))
            return false;
    return true;
}

// return a pointer to string "/proc/[dirname]/status"
const char* parsing_dirname(const char* dirname){
    static char path[_POSIX_PATH_MAX+10];

    sprintf(path, "/proc/%s/status", dirname);

    return path;
}

int main(int argc, const char *argv[]){

    if(argc != 2) usageErr("%s username", argv[0]);

    uid_t target_uid = get_uid(argv[1]);

    printf("User %s, uid = %ld\n", argv[1], (long)target_uid);
        
    // Open directory stream
    DIR *proc_dir = opendir("/proc");     
    if(proc_dir == NULL) errExit("opendir");

    // Loop each subdirectory
    for(struct dirent *subdir; (subdir = readdir(proc_dir)) != NULL ; ){

        // numberical only
        if(!isNumericalStr(subdir->d_name)) continue;
        
        // try to open /proc/[that_dir]/status. if failed, next one
        FILE* f = fopen(parsing_dirname(subdir->d_name), "r");
        if( f == NULL ) {errno = 0; continue;};

        // try to read content line by line. if EOF or failed, next one
        char buf[BUF_SIZE];
        char program_name[BUF_SIZE] = {0}; 
        pid_t pid = 0;
        uid_t uid = 0;

        // parsing content
        while(fgets(buf, BUF_SIZE, f)){

            if(strncmp(buf, "Name:", 5) == 0)
                sscanf(buf+5, " %s", program_name);

            else if(strncmp(buf, "Pid:",  4) == 0)
                sscanf(buf+4, " %d", &pid);

            else if(strncmp(buf, "Uid:",  4) == 0)
                sscanf(buf+4, " %d", &uid);
        }

        if(uid == target_uid && pid != 0)
            printf("Process %7ld %s\n", (long)pid, program_name);

        errno = 0;
    }
    if(errno != 0) errExit("readdir");

    return 0;
}
