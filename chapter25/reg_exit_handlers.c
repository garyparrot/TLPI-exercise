/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             reg_exit_handlers.c
*
* Author:           garyparrot
* Created:          2019/07/29
* Description:      Demonstrating atexit() and on_exit()
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

static void atexit1(void) { printf("atexit function 1 called\n"); }
static void atexit2(void) { printf("atexit function 2 called\n"); }
static void onexitfunc(int exitStatus, void* arg) {
    printf("on_exit function called: status=%d arg=%ld\n", exitStatus, (long)arg); 
}

int main(int argc, const char *argv[]){

    if(on_exit(onexitfunc, (void*)10) != 0) fatal("on_exit 1");
    if(atexit(atexit1) != 0) fatal("atexit1");
    if(atexit(atexit2) != 0) fatal("atexit2");
    if(on_exit(onexitfunc, (void*)20) != 0) fatal("on_exit 1");
    
    exit(2);
}
