/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_mount.c
*
* Author:           garyparrot
* Created:          2019/07/22
* Description:      demonstrating how to use mount 
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

void usageError(const char *program_name, const char *msg)
{
    if(msg != NULL) fprintf(stderr, "%s\n", msg);

    fprintf(stderr, "Usage: %s [options] source target\n\n", program_name);
    fprintf(stderr, "Available options:\n"
                    "    -t fstype          [e.g., 'ext2' or 'reiserfs']\n"
                    "    -o data            [file system-dependent options, e.g., 'bsdgroups' for ext2]\n"
                    "    -f mountflags      can include any of:\n"
                    "        b - MS_BIND            create a bind mount\n"
                    "        d - MS_DIRSYNC         synchronous direcory updates\n"
                    "        l - MS_MANDLOCK        permit mandatory locking\n"
                    "        m - MS_MOVE            atomically move subtree\n"
                    "        A - MS_NOATIME         don't update atime (last access time)\n"
                    "        V - MS_NODEV           don't permit device access\n"
                    "        D - MS_NODIRATIME      don't update atime on directories\n"
                    "        E - MS_NOEXEC          don't allow executables\n"
                    "        S - MS_NOSUID          disable set-user/group-ID programs\n"
                    "        r - MS_RDONLY          read-only mount\n"
                    "        c - MS_REC             recursive mount\n"
                    "        R - MS_REMOUNT         remount\n"
                    "        s - MS_SYNCHRONOUS     make writes synchronous\n"
                    );

    exit(EXIT_FAILURE);
}

extern char* optarg;

extern int optind;

int main(int argc, char* const *argv){

    int flags;
    char *fstype, *data;

    for(int opt; (opt = getopt(argc, argv, ":t:o:f:")) != -1; ){
        switch(opt){
            case 't': fstype = optarg; break;
            case 'o': data   = optarg; break;
            case 'f': 
                for(int i = 0, len = strlen(optarg); i < len ; i++)
                {
                    switch(optarg[i]){
                        case 'b': flags |= MS_BIND;       break;
                        case 'd': flags |= MS_DIRSYNC;    break;
                        case 'l': flags |= MS_MANDLOCK;   break;
                        case 'm': flags |= MS_MOVE;       break;
                        case 'A': flags |= MS_NOATIME;    break;
                        case 'V': flags |= MS_NODEV;      break;
                        case 'D': flags |= MS_NODIRATIME; break;
                        case 'E': flags |= MS_NOEXEC;     break;
                        case 'S': flags |= MS_NOSUID;     break;
                        case 'r': flags |= MS_RDONLY;     break;
                        case 'c': flags |= MS_REC;        break;
                        case 'R': flags |= MS_REMOUNT;    break;
                        case 's': flags |= MS_SYNCHRONOUS;break;
                        default:  usageError(argv[0], NULL);
                    }
                    break;
                }
            default:
                usageError(argv[0], NULL);
        }
    }

    if(argc != optind + 2)
        usageError(argv[0], "Dummy arguments.");

    const char *source = argv[optind];
    const char *target = argv[optind + 1];

    if(mount(source, target, fstype, flags, data) == -1)
        errExit("mount");
    
    return 0;
}
