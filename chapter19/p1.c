/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/24
* Description:      Chapter 19 Exercise 1
******************************************************************************/

#define _XOPEN_SOURCE 500
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ftw.h>
#include <string.h>
#include <sys/inotify.h>

#include "tlpi_hdr.h"

// Use inotify to monitor creat/delete/rename operation

long wd_max;
int ifd;
char **wd_filename_pair;

int setup_notify(const char *filename){
    // CREATE : tracing file file created
    // DELETE_SELF : file get deleted
    // MOVE_SELF : file renamed
    // MOVE_TO : New File
    // MOVE_FROM : File exit.
    int wd = inotify_add_watch(ifd, filename, IN_CREATE | IN_DELETE_SELF | IN_MOVE_SELF | IN_MOVE |  IN_DONT_FOLLOW);
    if(wd == -1) return -1;

    wd_filename_pair[wd] = strdup(filename);
    return 1;
}

void remove_notify(int wd){
    if(wd_filename_pair[wd] == NULL) return;
    if(inotify_rm_watch(ifd, wd) == -1);
    free(wd_filename_pair[wd]);
    wd_filename_pair[wd] = NULL;
}

const char* merge_name(const char* first, const char* second){
    static char buf[_PC_PATH_MAX];
    sprintf(buf, "%s/%s", first, second);
    return buf;
}

int travesal(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
    
    if(setup_notify(fpath) == -1)
        printf("Cannot setup notify for %s.", fpath);

    return 0;
}

int main(int argc, const char *argv[]){

    if(argc != 2) usageErr("Usage: %s directory", argv[0]);
    
    ifd = inotify_init();
    if(ifd == -1) errExit("inotify_init");

    // limit wd
    FILE* f = fopen("/proc/sys/fs/inotify/max_user_watches", "r");
    fscanf(f,"%ld", &wd_max);

    // initialize index table
    wd_filename_pair = malloc(sizeof(char*) * wd_max);
    memset(wd_filename_pair, 0, sizeof(char*) * wd_max);

    /* setup inotify for each file under the directory */
    nftw(argv[1], travesal, sysconf(_SC_OPEN_MAX) - 5, 0);

    /* listening event */
    ssize_t rsize;
    static char buf[4096];
    uint32_t lastfc = 0, lasttc = 0;        /* cookie of last from and last to */
    uint32_t lastfwd = 0,lasttwd = 0;       /* wd of last from and last to     */
    static char fname[_PC_NAME_MAX+100];
    static char tname[_PC_NAME_MAX+100];    /* from name and to name           */

    for(;;){
        if((rsize = read(ifd, buf, 4096)) == -1) errExit("read");

        for(void* p = (void*)buf ; p < (void*)buf + rsize ;){

            struct inotify_event *event = p;

            const char *oldname = wd_filename_pair[event->wd];
            const char *newname = event->name;

            static char timebuf[256];
            time_t tt = time(NULL);
            strftime(timebuf, 256, "%F %T", localtime(&tt));

            if(event->mask & IN_CREATE){
                printf("[ %s ] New file '%s' was created under '%s'.\n", timebuf, newname, oldname);
                if(setup_notify(merge_name(oldname, newname)) == -1) printf("Cannot setup notify for %s.", event->name);
            }
            if(event->mask & IN_DELETE_SELF){
                printf("[ %s ] file '%s' was deleted.\n", timebuf, oldname);
                remove_notify(event->wd);
            }
            // this event will trigger after from or to event.
            // with this factor, we can check if last from / to event is rename or not
            if(event->mask & IN_MOVE_SELF){
                if(lastfwd != 0 && lastfwd == lasttwd && lastfc == lasttc){ /* rename */
                    printf("[ %s ] File %s was rename as %s.\n", timebuf, fname, tname);
                    free(wd_filename_pair[event->wd]);
                    wd_filename_pair[event->wd] = strdup(tname);
                    lastfc = lasttc = lastfwd = lasttwd = 0;
                }
                if(lastfwd != 0){
                    remove_notify(event->wd);
                    printf("[ %s ] File %s move to another directory.\n", timebuf, fname);
                }
            }
            if(event->mask & IN_MOVED_FROM){
                lastfwd = event->wd;
                lastfc  = event->cookie;
                strcpy(fname, merge_name(wd_filename_pair[event->wd],event->name));
            }
            if(event->mask & IN_MOVED_TO){
                lasttwd = event->wd;
                lasttc  = event->cookie;
                strcpy(tname, merge_name(wd_filename_pair[event->wd],event->name));
                setup_notify(tname);
                if(lasttwd != event->wd)
                    printf("[ %s ] File %s move in from elsewhere.\n", timebuf, tname); 
            }
            if(event->mask & IN_IGNORED || event->mask & IN_UNMOUNT){
                remove_notify(event->wd);
            }
        
            p += sizeof(struct inotify_event) + event->len;
        }

    }

    return 0;
}
