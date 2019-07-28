/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             timed_read.c
*
* Author:           garyparrot
* Created:          2019/07/28
* Description:      You have 20 seconds to comply.
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "tlpi_hdr.h"

void handler(int sig){ }

int main(int argc, const char *argv[]){

    setbuf(stdout, NULL);

    struct sigaction siga;
    siga.sa_handler = handler;
    siga.sa_flags   = 0;
    sigemptyset(&siga.sa_mask);

    sigaction(SIGALRM, &siga, NULL);

    printf("You have 20 seconds to comply.\n");
    printf("Password: ");

    alarm(20);

    char buf[12];
    ssize_t readed = read(STDIN_FILENO, buf, 12);

    int sav_errno = errno;

    /* put off the alarm */
    alarm(0);

    if(readed == -1){
        if(sav_errno == EINTR)
            printf("\nTime out\nPew Pew Pew Pew Pew Pew\n");
        else
            errExit("read");
    } else {
        buf[readed-1] = '\0';
        if(strcmp(buf, "greedisgood") == 0)
            printf("Access granted.");
        else
            printf("\nWrong Pass\nPew Pew Pew Pew Pew Pew\n");
    }
    
    return 0;
}
