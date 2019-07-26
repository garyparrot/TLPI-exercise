/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             signal_functions.c
*
* Author:           garyparrot
* Created:          2019/07/26
* Description:      Demonstrating sigsetjmp / siglongjmp
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <setjmp.h>

#include "tlpi_hdr.h"
#include "signal_functions.h"

// sigsetjmp will set mask back after returing from signal.
// sigsetjmp(sigjmp_buf env, int savesigs)
// if savesigs not zero, it will store current mask. and one longjmp happened, jump back.

#ifdef SIGSETJMP
static sigjmp_buf sjmpbuf;
#else
static jmp_buf jmpbuf;
#endif

// volatile avoid optimization
// sig_atomic_t to ensure operation is atomic
static volatile sig_atomic_t canJmp;

void handler(int sig){
    printf("Signal received (%d)\n", sig);

#ifdef SIGSETJMP
    siglongjmp(sjmpbuf,1);
#else
    longjmp(jmpbuf,1);
#endif 

}

int main(int argc, const char *argv[]){

    struct sigaction sgaction;
    sgaction.sa_handler = handler;
    sigemptyset(&sgaction.sa_mask);
    sgaction.sa_flags = 0;

    sigaction(SIGINT, &sgaction, NULL);

#ifdef SIGSETJMP
    if(sigsetjmp(sjmpbuf, 1) == 0)
#else
    if(setjmp(jmpbuf) == 0)
#endif
        canJmp = 1;
    else{
        printSigMask(stdout,"Jump from signal, current mask:");
    }

    for(;;)
        pause();
    
    return 0;
}
