/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p5.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      Chapter 18 Exercise 5
******************************************************************************/

#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"


static void usageError(const char *progName, const char *msg){
    if(msg != NULL)
        fprintf(stderr, "%s\n", msg);
    fprintf(stderr, "Usage: %s [-d] [-m] [-p] [directory-path]\n", progName);
    fprintf(stderr, "\t-d Use FTW_DEPTH flag\n");
    fprintf(stderr, "\t-m Use FTW_MOUNT flag\n");
    fprintf(stderr, "\t-p Use FTW_PHYS  flag\n");
    exit(EXIT_FAILURE);
}

int regCount, dirCount, symCount;

static int dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb){
    if(S_ISREG(sbuf->st_mode)) regCount++;
    if(S_ISDIR(sbuf->st_mode)) dirCount++;
    if(S_ISLNK(sbuf->st_mode)) symCount++;

    return 0;
}

int main(int argc, const char *argv[]){

    int flags = 0, opt;

    while((opt = getopt(argc, argv, "dmp")) != -1){
        switch(opt){
            case 'd': flags |= FTW_DEPTH; break;
            case 'm': flags |= FTW_MOUNT; break;
            case 'p': flags |= FTW_PHYS;  break;
            deafult:  usageError(argv[0], NULL); break;
        }
    }

    if(argc > optind + 1)
        usageError(argv[0], NULL);

    if(nftw((argc > optind) ? argv[optind] : "." , dirTree, 10, flags) == -1){
        perror("nftw");
        exit(EXIT_FAILURE);
    }

    double sum = regCount + dirCount + symCount;
    printf("Regular   file : %8d (%6.3lf%%)\n", regCount, (regCount) / sum * 100);
    printf("Directory file : %8d (%6.3lf%%)\n", dirCount, (dirCount) / sum * 100);
    printf("Symbolic  file : %8d (%6.3lf%%)\n", symCount, (symCount) / sum * 100);
    
    return 0;
}
