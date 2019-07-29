/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             testing_signal_abort.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Signal terminated process won't trigger registed exit functions
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

void die(void){
    printf("I don't want to die...\n");
}

int main(int argc, const char *argv[]){

    atexit(die);
 
    // Send any signal, die function won't trigger.
    pause();
    
    return 0;
}
