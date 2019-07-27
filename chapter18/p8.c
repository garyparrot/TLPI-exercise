/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p8.c
*
* Author:           garyparrot
* Created:          2019/07/26
* Description:      Chapter 18 Exercise 8, Implement it
******************************************************************************/

// Flags FTW_PHYS  done
// Flags FTW_DEPTH done
// Flags FTW_MOUNT done
// Flags FTW_CHDIR not implement, client please do it your self :pp

#define _GNU_SOURCE
#define _XOPEN_SOURCE 500
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include "tlpi_hdr.h"

#define FAILURE errExit("Failure %s(%d)", __FILE__, __LINE__);
#define STUB if(errno != 0){  errExit("%s %d", __FILE__, __LINE__); }
#define close_fd(seq) do { close(opened_fd[seq]); opened_fd[seq_fd] = -1; } while(0)

struct FTW{ int base; int level; };

enum { 
    FTW_DEPTH=1,
    FTW_PHYS =2,
    FTW_MOUNT=4,
    FTW_CHDIR=8
} FTW_FLAGS;
enum {
    FTW_F    =1,
    FTW_D    =2,
    FTW_DNR  =3,
    FTW_DP   =4,
    FTW_NS   =5,
    FTW_SL   =6,
    FTW_SLN  =7 
} FTW_TYPE;

// to do : handle fn return value
int nftw(const char *dirpath, 
         int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf),
         int nopenfd,
         int flags
        ){
    /* 
     * This function maintain a nftw_running variable, it stands for is this function running.
     * If so, initialize other variables. Once nftw done its jobs. It should set nftw_running
     * to zero again.
     * 
     * Each Iteration will have a seq_fd number which stands for index of opened_fd for this directory or file
     * If there is no more available index for use, it will release the last openend fd(usually its
     * original fd number)
     *
     * Once iteration finished, if current recursion find out its fd has been released. It will reopen it
     * and continue iteration.
     */
    static int origin_fd;       /* Preserve source fd */
    static int nftw_running;    /* Indicate that nftw is running */
    static char *current_path;  /* Current iteration path */
    static int* opened_fd;      /* Opened fd */
    static int last;            /* Last unused or last used index */
    static int level;           /* depth */
    static dev_t cur_dev;       /* The dev number of beginning dir */
    int seq_fd;                 /* Sequence number of current opened_fd array */
    int first_iteration = 0;    /* Indicate this recursion is the first recursion, it should free resource */

    if(nftw_running == 0){
        nftw_running = 1; level = 0; first_iteration = 1;
        current_path = malloc(sizeof(char) * 1024);
        strcpy(current_path, dirpath);
        opened_fd = malloc( sizeof(int*) * nopenfd);
        memset(opened_fd, -1, sizeof(int*) * nopenfd);
        // FTW_MOUNT. get dev number of beginning file/dir
        if(flags & FTW_MOUNT)
        {
            int tfd = open(current_path, O_RDONLY | O_PATH | O_NOFOLLOW);
            struct stat tt;
            fstat(tfd, &tt);
            cur_dev = tt.st_dev;
            close(tfd);
        }
    }

    seq_fd = (last); last = (last + 1) % nopenfd; level++;

    if(opened_fd[seq_fd] != -1) close_fd(seq_fd);

    int typeflag;
    int no_permission = 0;

    // Since we have cannot extract the symbolic link itself, we are goin to open the symbolic link itself
    // O_PATH | O_NOFOLLOW stands for don't extract the link
    // But there is a side effect, we cannot read this file descriptor
    // So later on we will reopen it if possible
    opened_fd[seq_fd] = open(current_path, O_RDONLY | O_PATH | O_NOFOLLOW);
    /* This file already disappear */
    if(opened_fd[seq_fd] == -1){ errno = 0; return 0; }

    struct stat cur, lnk;
    
    // procedure filter
    if(fstat(opened_fd[seq_fd], &cur) == -1) { errno = 0; typeflag = FTW_NS; goto visite; }
    // Stay in the same device
    if(flags & FTW_MOUNT && cur.st_dev != cur_dev) return 0;

    switch(cur.st_mode & S_IFMT){
        case S_IFREG:
        case S_IFBLK:
        case S_IFCHR:
        case S_IFIFO:
        case S_IFSOCK:
            typeflag = FTW_F;   /* Just ... file */
            break;
        case S_IFDIR:
            typeflag = (access(current_path, R_OK | X_OK | F_OK) != 0) ? FTW_DNR :  /* Cannot read the content of dir */
                       (flags & FTW_DEPTH) ? FTW_DP : FTW_D;          /* We should do this directory pre or post */
            no_permission = (errno == EPERM);
            errno = 0;
            break;
        case S_IFLNK:
            typeflag = (stat(current_path, &lnk) != 0) ? FTW_SLN :         /* Is this link point to nowhere */
                       (flags & FTW_PHYS)              ? FTW_SL :          /* Is Nofollow specified */
                       (S_ISDIR(lnk.st_mode))          ? ( (flags & FTW_DEPTH ? FTW_DP : FTW_D) ) :  /* Link to a dir */
                       (S_ISREG(lnk.st_mode))          ? FTW_F : FTW_F;    /* Just a file */
            errno = 0;
            break;
        default:
            fatal("This should never happened.");
    }

visite:

    // This step reopen the file descriptor without O_PATH | O_NOFOLLOW
    // But if we don't have the permission to reading / executing the directory
    // We won't do that, otherwise we will get a EPERM error
    if(typeflag != FTW_DNR && typeflag != FTW_SLN && (typeflag == FTW_DP || typeflag == FTW_D)){
        close_fd(seq_fd);
        opened_fd[seq_fd] = open(current_path, O_RDONLY);
        no_permission = (errno == EPERM);
        errno = 0;
    }

    struct FTW ftwbuf = {
        .base  = strlen(current_path) - (int)(strrchr(current_path, '/') - current_path + 1),
        .level = level
    };


    /* Do it pre-order */
    int ret = 0;
    if(!(flags & FTW_DEPTH)) {
        int ret = fn(current_path, &cur, typeflag, &ftwbuf);
        // if(flags & FTW_CHDIR) fchdir(origin_fd);
        if(ret != 0) goto clean_up;
    }


    /* Visit content of directory , also don't vista no permission dir*/
    if((typeflag == FTW_DP || typeflag == FTW_D) && !no_permission){
        DIR* dir;
        long seek_val = 0;

        dir = fdopendir(opened_fd[seq_fd]);
        if(dir == NULL) errExit("%s %d : fdopendir", __FILE__, __LINE__);

        /* Open this directory if it is not opened or has been released */
        struct dirent *direntry;
        while((direntry = readdir(dir)) != NULL){
            seek_val = telldir(dir);
            /* Modify the content of current_path */
            char *save_end = current_path + strlen(current_path);
            if(strcmp(direntry->d_name, "..") == 0) continue;
            if(strcmp(direntry->d_name, ".")  == 0) continue;

            // append next file name
            if(current_path[strlen(current_path)-1] != '/') strcat(current_path, "/");
            strcat(current_path, direntry->d_name);
            
            ret = nftw(dirpath, fn, nopenfd, flags);
            if(ret != 0) goto clean_up;

            *save_end = '\0';

            if(opened_fd[seq_fd] == -1){
                closedir(dir);
                if(opened_fd[seq_fd] != -1) fatal("%s %d", __FILE__, __LINE__);
                opened_fd[seq_fd] = open(current_path, O_RDONLY);  /* Should be ok to follow */
                dir = fdopendir(opened_fd[seq_fd]);
                seekdir(dir, seek_val);
            }
        }
        /* Bad directory stream, nothing */
        if(errno == EBADF) errno = 0;
        /* Unexpected Error, since we cannot access it just ignore */
        if(errno != 0) errno = 0;

    }

    /* do it post-order */
    if( (flags & FTW_DEPTH))
    {
        ret = fn(current_path, &cur, typeflag, &ftwbuf);
        if(flags & FTW_CHDIR) fchdir(origin_fd);
        if(ret != 0) goto clean_up;
    }

clean_up:

    if(opened_fd[seq_fd] != -1) close_fd(seq_fd);
    last = (last - 1 + nopenfd) % nopenfd; level--;

    if(first_iteration){
        free(opened_fd);
        free(current_path);
        nftw_running = 0;
        last = 0;
    }

    return ret;
}

int func(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
    printf("%s\n", fpath);

    return 0;
}

int main(int argc, const char *argv[]){

    int ret = nftw(argv[1], func, 25, FTW_DEPTH | FTW_CHDIR | FTW_MOUNT);
    printf("ret value = %d\n", ret);
    
    return 0;
}
