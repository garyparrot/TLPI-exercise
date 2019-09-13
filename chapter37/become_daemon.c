#include <syslog.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tlpi_hdr.h"

#define BD_NO_CHDIR             01
#define BD_NO_CLOSE_FILES       02
#define BD_NO_REOPEN_STD_FDS    04
#define BD_NO_UMASK0           010
#define BD_MAX_CLSOE          8192

int becomeDaemon(int flags){
    /* strip current process group */
    switch(fork()){
        case -1: 
            return -1;
        case  0:
            break;
        default:
            _exit(EXIT_SUCCESS);
    }

    /* strip current session by setsid() */
    if(setsid() == -1)
        return -1;

    /* Now this process is complete free from shell */

    switch(fork()){
        case -1:
            return -1;
        case  0:
            break;
        default:
            _exit(EXIT_SUCCESS);
    }

    if(!(flags & BD_NO_UMASK0)) umask(0);
    if(!(flags & BD_NO_CHDIR))  chdir("/");
    if(!(flags & BD_NO_CLOSE_FILES)){
        int maxfd = sysconf(_SC_OPEN_MAX);
        if(maxfd == -1)
            maxfd = BD_MAX_CLSOE;

        for(int fd = 0; fd < maxfd; fd++)
            close(fd);
    }

    if(!(flags & BD_NO_REOPEN_STD_FDS)){
        close(STDIN_FILENO);

        int fd = open("/dev/null", O_RDWR);

        if(fd != STDIN_FILENO)  /* should be zero */
            return -1;
        if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -1;
        if(dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -1;

    }
    return 0;
}

int main(int argc, char *argv[])
{
    if(becomeDaemon(0) != 0)
        errExit("becomeDaemon");

    alarm(60);

    pause();

    return 0;
}
