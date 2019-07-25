/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_raise.c
*
* Author:           garyparrot
* Created:          2019/07/25
* Description:      Demonstrating raise()
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

// raise() send a signal to the process self.
// it handle the signal before the function return since this is a interrupt.

int main(int argc, const char *argv[]){

    //raise(sig) equal to kill(getpid(), sig) 
    //raise(sig) equal to pthread_kill(pthread_self(), sig) for multi-thread system

    // Quit myself
    raise(SIGQUIT);
    
    return 0;
}
