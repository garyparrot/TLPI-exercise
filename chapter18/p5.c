/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p5.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      Chpater 18 Exercise 5 Implement getcwd()
******************************************************************************/

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

char *getcwd(char *buf, size_t size){
    
    buf[0] = '\0';

    /* keep go back until hit the root */
    int len = 0;
    char dir[512];

#define equal_inode(a,b) (a.st_dev == b.st_dev && a.st_ino == b.st_ino)
#define FAILURE   errExit("%s %d",__FILE__, __LINE__);
#define DETAIL_FAILURE errExit("%s %d: %s %s", __FILE__, __LINE__, dbuf, buf)
    dir[0] = '.'; dir[1] = '/'; dir[2] = '\0'; len = 2;
    buf[0] = '\0';
    while(1){
        // ./../../../../../
        struct stat current, parent;
        dir[len++] = '.'; dir[len] = '\0';
        if(stat(dir, &current) == -1) FAILURE;
        dir[len++] = '.'; dir[len++] =  '/', dir[len] = '\0';
        if(stat(dir, &parent)  == -1) FAILURE;

        if(equal_inode(current, parent)){
            // ../../../
            dir[len -= 3] = '\0';
            break;
        }
    }
    if(len == 2){
        strcat(buf, "/");
        return buf;
    }


    while(1){
        if(len == 2) break;

        DIR* parent;
        struct stat sub, opened;

        if((parent = opendir(dir)) == NULL) FAILURE;
        len -= 3;
        dir[len] = '\0';
        if(stat(dir, &sub) == -1) FAILURE;

        struct dirent *item;
        while((item = readdir(parent)) != NULL){
            static char dbuf[1024]; dbuf[0] = '\0';
            strcat(dbuf, dir);
            strcat(dbuf, "../");
            strcat(dbuf, item->d_name);
            if(stat(dbuf, &opened) == -1) continue;
            if(equal_inode(opened, sub))
                break;
        }

        if(item == NULL) FAILURE;
        int len = strlen(item->d_name);
        if(len + 2 > size) FAILURE;         /* overflow */
        strcat(buf, "/");
        strcat(buf, item->d_name);              /* /old/path/and/newfile */
    }

    return buf;
}



int main(int argc, const char *argv[]){

    char buf[1000];
    if(getcwd(buf, 1000) == NULL) errExit("getcwd");
    printf("%s\n", buf);
    
    
    return 0;
}
