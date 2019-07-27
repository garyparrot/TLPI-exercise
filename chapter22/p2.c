/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/07/27
* Description:      Chapter 22 Exercise 2
******************************************************************************/

/*
 * Result: Feels like Real time signal go first 
NSIG=65 RTMIN=34 RTMAX=64
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
1
2
3
4
5
7
8
10
11
12
13
14
16
17
18
21
22
23
24
25
26
27
28
29
30
31
*/

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
    printf("%d\n", sig);
}

int main(int argc, const char *argv[]){

    printf("NSIG=%d RTMIN=%d RTMAX=%d\n", NSIG, SIGRTMIN, SIGRTMAX);

    sigset_t st;
    sigfillset(&st);

    // init
    struct sigaction siga;
    siga.sa_handler = handler;
    sigemptyset(&st);
    siga.sa_flags = 0;

    // signal handler
    for(int i = 1; i < NSIG; i++)
        if(i != SIGKILL && i != SIGTERM)
            if(sigaction(i, &siga, NULL) == -1)
                errno = 0;
    
    // Mask every signal
    if(sigprocmask(SIG_BLOCK, &st, NULL) == -1)   errExit("sigprocmask");

    // raise every signal
    union sigval val = { .sival_int = 0 };
    for(int i = SIGRTMIN; i <= SIGRTMAX; i++ )
        sigqueue(getpid(), i, val);
    for(int i = 1; i < 32; i++)
        if(i != SIGTSTP && i != SIGABRT && i != SIGSTOP && i != SIGKILL && i != SIGTERM)
            raise(i);

    // sleep for a while
    sleep(1);

    // Release mask
    if(sigprocmask(SIG_UNBLOCK, &st, NULL) == -1) errExit("sigprocmask");

    return 0;
}
