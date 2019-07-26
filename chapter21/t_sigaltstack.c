/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_sigaltstack.c
*
* Author:           garyparrot
* Created:          2019/07/26
* Description:      Demonstrating sigaltstack
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

void handler(int sig){
    int top;
    printf("Handle this, %10p\n", &top);

    _exit(EXIT_FAILURE);
}

void overflow(int call){
    int arr[100000];
    int top;

    printf("[%5d] Current stack top %10p\n", call, &top);

    overflow(call+1);
}

int main(int argc, const char *argv[]){

    // creat alternative stack for signal
    stack_t sigstack;
    sigstack.ss_sp = malloc(SIGSTKSZ);
    if(sigstack.ss_sp == NULL) 
        errExit("malloc");
    sigstack.ss_flags = 0;
    sigstack.ss_size  = SIGSTKSZ;
    
    if(sigaltstack(&sigstack, NULL) == -1)
        errExit("sigaltstack");

    printf("Alternative Stack size = %ld\n", (long)SIGSTKSZ);
    printf("Alternative stack pos [%10p, %10p)\n", (sigstack.ss_sp), sbrk(0));
    
    // registe signal handler
    struct sigaction sigact;
    sigact.sa_handler = handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_ONSTACK;   /* IMPORTANT: enable this flag to run it on stack */
    
    if(sigaction(SIGSEGV, &sigact, NULL) == -1)
        errExit("sigaction");
    

    // try to blow the stack
    overflow(1);

    return 0;
}
