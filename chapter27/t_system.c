/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_system.c
*
* Author:           garyparrot
* Created:          2019/07/31
* Description:      Demonstrate system()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "print_wait_status.h"
#include "tlpi_hdr.h"

#define MAX_CMD_LED 200

int main(int argc, const char *argv[]){

    char str[MAX_CMD_LED];
    int status;

    for(;;){
        printf("Command: ");
        fflush(stdout);

        if(fgets(str, MAX_CMD_LED, stdin) == NULL)
            break;

        status = system(str);
        printf("system() returned: status=0x%04x (%d,%d)\n", (unsigned int) status, status >> 8, status & 0xff);

        if(status == -1)
            errExit("system");
        else{
            if(WIFEXITED(status) && WEXITSTATUS(status) == 127)
                printf("(Probably) could not invoke shell\n");
            else
                printWaitStatus(NULL, status);
        }
    }
    
    exit(EXIT_SUCCESS);
}
