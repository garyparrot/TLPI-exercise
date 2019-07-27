/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p3.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      Implement realpath()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include "tlpi_hdr.h"

char *realpath_w(const char *path, char* resolved_path){
    if(path[0] == '/')
        // absolute path /path/to/somewhere
        resolved_path[0] = '/', resolved_path[1] = '\0', path++;

    // resolved_path -> /a/b/c/d
    // path -> a/b/c///d/e/f
    const char* scan = path;
    while(true){
        const char* next_slash = strchr(scan, '/'); 
        if(next_slash == NULL) next_slash = scan + strlen(scan); // encounter \0
        size_t len = (next_slash - scan);

        if(len == 2 && scan[0] == '.' && scan[1] == '.'){;
            char* last_slash = strrchr(resolved_path, '/');
            if(last_slash == resolved_path) *(last_slash+1) = '\0';
            else *last_slash = '\0';
        }else if(len == 1 && scan[0] == '.'){
            ;
        }else if(len == 0){
            ;
        }else{
            struct stat buf; 
            if(resolved_path[1] != '\0') strcat(resolved_path, "/");
            strncat(resolved_path, scan, len);
            if(lstat(resolved_path, &buf) == -1) return NULL;

            if(S_ISREG(buf.st_mode)){
                return (*next_slash == '\0') ? resolved_path : NULL;
            }else if(S_ISDIR(buf.st_mode)){
                if(*next_slash == '\0') return resolved_path;
            }else if(S_ISLNK(buf.st_mode)){
                char link[PATH_MAX];
                readlink(resolved_path, link, PATH_MAX);
                *strrchr(resolved_path, '/') = '\0';
                if(realpath_w(link, resolved_path) == NULL) 
                    return NULL;
            }else{
                return NULL;
            }
        }

        if(*next_slash == '\0')
            break;
        scan = next_slash + 1;
    }

    return resolved_path;
}

char *realpath(const char* path, char* resolved_path){
    if(path[0] != '/')
        getcwd(resolved_path, PATH_MAX);

    return realpath_w(path, resolved_path);
}

int main(int argc, const char *argv[]){

    char rpath[PATH_MAX];
    char *path = realpath(argv[1], rpath);
    printf("%s\n", path == NULL ? "ERROR" : path);

    return 0;
}
