/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             make_zombie.c
*
* Author:           garyparrot
* Created:          2019/07/30
* Description:      Demonstrating how to create a zombie process.
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <libgen.h>

#include "tlpi_hdr.h"

#define CMD_SIZE 200

/* If parent process pass out before child process do, child will become a orphans */
/* Process with PID 1 will always accept orphans process. so kind. */

/* If child process exit before parent call wait() function will become a zombie process */
/* Since system allow parent process wait() for any process at any time, it is legal to wait() even it is dead. */
/* System will release most of the resource it(child process) allocated, kernel just preserve some of the record */
/* e.g. Child PID, Exit Status, Resource usage info ... */
/* For these process who passed before its parent call wait(). We call it zombie process */
/* Because parent process might need some information about the exited process, kernel cannot just delete it */
/* So kernel make it a zombie(process already dead but still has some record in kernel's datastruct)  */
/* Also, because that process already dead. Event SIGKILL cannot remove this process. Because it is already dead */

/* The only way to remove zombie process is wait for the parent process call wait() for it */
/* Or more brute, kill the parent process. */

/* Zombie process will keep kernel maintain its record. if there is too many zombie process,
 * kernel PID table will run out of space for new process */

int main(int argc, char *argv[]){

    char cmd[CMD_SIZE];
    pid_t childPid;

    setbuf(stdout, NULL);

    printf("Parent PID=%ld\n", (long)getpid());

    switch(childPid = fork()){
        case -1: errExit("fork");
        case  0:
            printf("Child (PID=%ld) exiting\n", (long) getpid());
            _exit(EXIT_SUCCESS);
        default:
            sleep(3);
            snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
            system(cmd);

            if(kill(childPid, SIGKILL) == -1)
                errMsg("kill");

            sleep(3);
            printf("After sending SIGKILL to zombie (PID=%ld):\n", (long) childPid);
            system(cmd);

            exit(EXIT_SUCCESS);
    }

    
    return 0;
}
