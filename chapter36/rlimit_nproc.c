#include <sys/resource.h>
#include "print_rlimit.h"
#include "tlpi_hdr.h"

int main(int argc, char *argv[]){
    if(argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s soft-limit [hard-limit]\n", argv[0]);

    printRlimit("Initial maximum process limits: ", RLIMIT_NPROC);
    
    struct rlimit rl;
    rl.rlim_cur = (argv[1][0] == 'i') ? RLIM_INFINITY : get_int(argv[1], GN_NOFLAG, GN_BASE10);
    rl.rlim_max = (argc == 2) ? rl.rlim_cur : (argv[2][0] == 'i') ? RLIM_INFINITY : get_int(argv[2], GN_NOFLAG, GN_BASE10);

    if(setrlimit(RLIMIT_NPROC, &rl) == -1)
        errExit("setrlimit");

    printRlimit("New maximum process limits: ", RLIMIT_NPROC);

    for(int j = 1; ; j++){
        pid_t child;
        switch(child = fork()){
            case -1: errExit("fork");
            case  0: _exit(EXIT_SUCCESS);
            default:
                printf("Child %d (PID=%ld) started\n", j , (long)child);
                break;
        }
    }


}
