#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

int system(const char *command){
    struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
    sigset_t blockMask, origMask;
    pid_t childPid;
    int status, savedErrno;

    if(command == NULL)
        /* default behavior of system(). if command is NULL, determine if shell is available */
        return system(":") == 0;        

    /* Block SIGCHLD, child process will inherited signal mask. */
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &blockMask, &origMask);

    /* Ignore SIGINT and SIGQUIT */
    /* Avoid this process get killed by group signal */
    saIgnore.sa_handler = SIG_IGN;
    saIgnore.sa_flags   = 0;
    sigemptyset(&saIgnore.sa_mask);
    sigaction(SIGINT, &saIgnore, &saOrigInt);
    sigaction(SIGQUIT, &saIgnore, &saOrigQuit);

    switch(childPid = fork()){
        case -1: status = -1; break;
        case  0:
            /*  */
            saDefault.sa_handler = SIG_DFL;
            saDefault.sa_flags   = 0;
            sigemptyset(&saDefault.sa_mask);

            /* Reset handler, new process deserve a clean config */
            if(saOrigInt.sa_handler != SIG_IGN)
                sigaction(SIGINT, &saDefault, NULL);
            if(saOrigQuit.sa_handler != SIG_IGN)
                sigaction(SIGQUIT, &saDefault, NULL);

            /* Reset mask, SIGCHLD is not require for new process */
            sigprocmask(SIG_SETMASK, &origMask, NULL);

            execl("/bin/sh", "sh", "-c", command, (char*)NULL);
            _exit(127);
        default:
            /* Avoid calling waitpid with SIG_IGN for SIGCHLD */
            /* SUSv3 require SIGCHLD with SIG_IGN will release status and other resource usage info */
            /* Wait for childPid terminated */
            while(waitpid(childPid, &status, 0) == -1){
                if(errno != EINTR){
                    status = -1;
                    break;
                }
            }
            break;
    }

    /* Ublock SIGCHLD restore dispositions of SIGINT and SIGQUIT */

    savedErrno = errno;

    /* Reset mask and other setting */
    sigprocmask(SIG_SETMASK, &origMask, NULL);
    sigaction(SIGINT, &saOrigInt, NULL);
    sigaction(SIGQUIT, &saOrigQuit, NULL);

    errno = savedErrno;

    return status;
}
