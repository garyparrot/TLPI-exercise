/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p4.c
*
* Author:           garyparrot
* Created:          2019/07/27
* Description:      Chapter 22 Exercise 4
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

#define SIG_HOLD (sighandler_t)(87)

typedef void (*sighandler_t)(int);

sighandler_t sigset(int sig, sighandler_t disp){

    sigset_t sigset;
    sigprocmask(SIG_BLOCK, NULL, &sigset);

    int oblocked = sigismember(&sigset, sig);

    if(disp == SIG_HOLD){
        // mask on 
        sigemptyset(&sigset);
        sigaddset(&sigset, sig);
        sigprocmask(SIG_BLOCK, &sigset, NULL);
    }else{
        // test if sig is blocked. if so, release its mask
        if(disp != SIG_HOLD && sigismember(&sigset, sig)){
            sigemptyset(&sigset);
            sigaddset(&sigset, sig);
            sigprocmask(SIG_UNBLOCK, &sigset, NULL);
        }

        // setup handler
        struct sigaction siga;
        siga.sa_handler = disp;
        sigemptyset(&siga.sa_mask);
        siga.sa_flags = 0;
        sigaction(sig, &siga, NULL);
    }

    return (oblocked) ? SIG_HOLD : disp;
}

int sighold(int sig){
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, sig);

    if(sigprocmask(SIG_BLOCK, &sigset, NULL) == -1) return -1;

    return 0;
}
int sigrelse(int sig){
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, sig);

    if(sigprocmask(SIG_UNBLOCK, &sigset, NULL) == -1) return -1;

    return 0;
}
int sigignore(int sig){
    struct sigaction siga;
    if(sigaction(sig, NULL, &siga) == -1) return -1;
    siga.sa_handler = SIG_IGN;
    if(sigaction(sig, &siga, NULL) == -1) return -1;
    return 0;
}

int sigpause(int sig){
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, sig);

    sigprocmask(SIG_BLOCK, &sigset, NULL);
    sigwaitinfo(&sigset, NULL);
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);
    return -1;
}

void handler(int sig){
    printf("\n SIGINT %d \n", sig);
}

int main(int argc, const char *argv[]){

    int num = atoi(argv[1]);

    switch(num){
        case 0:
            printf("Set handler for SIGINT signal.\n");
            sigset(SIGINT, handler);
            pause();
            break;
        case 1:
            printf("Set Dfeault handler for SIGINT signal.\n");
            sigset(SIGINT, SIG_DFL);
            pause();
            break;
        case 2:
            printf("Ignore SIGINT signal.\n");
            sigset(SIGINT, SIG_IGN);
            pause();
            break;
        case 3:
            printf("Set handler for SIGINT signal and then release it.\n");
            sigset(SIGINT, handler);
            sighold(SIGINT);
            sigrelse(SIGINT);
            pause();
            break;
        case 4:
            printf("Set mask for SIGINT signal.\n");
            sigset(SIGINT, handler);
            sighold(SIGINT);
            pause();
            break;
        case 5:
            printf("Ignore SIGINT signal.\n");
            sigset(SIGINT, handler);
            sigignore(SIGINT);
            pause();
            break;
        case 6:
            printf("HOLD \n");
            sigset(SIGINT, handler);
            sigset(SIGINT, SIG_HOLD);
            pause();
            break;
        case 7:
            printf("Sigpaues, please send SIGINT to continue.\n");
            sigpause(SIGINT);
            printf("You made it\n");
    }
    
    return 0;
}
