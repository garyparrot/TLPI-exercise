#include <string.h>
#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

void printWaitStatus(const char *msg, int status){
    if(msg != NULL)
        printf("%s\n", msg);

    /* test if process exited */
    if(WIFEXITED(status))
        printf("Child exited, status=%d\n", WEXITSTATUS(status));
    else if(WIFSIGNALED(status)){
        printf("Child killed by signal %d (%s)", WTERMSIG(status), strsignal(WTERMSIG(status)));
#ifdef WCOREDUMP  /* SUSv3 didn't specific WCOREDUMP, but most UNIX implementation do. */
        if(WCOREDUMP(status))
            printf(" (core dumped)");
#endif
        printf("\n");
    }else if(WIFSTOPPED(status)){
        printf("child stopped by signal %d (%s)\n", WSTOPSIG(status), strsignal(WSTOPSIG(status)));

#ifdef WIFCONTINUED  /* Supported since Linux 2.6.10 */
    } else if(WIFCONTINUED(status)){
        printf("child continued\n");
#endif
    }else{
        /* This should never happened. */
        printf("what happened to this child? (status=%x)\n", (unsigned int) status);
    }
}
