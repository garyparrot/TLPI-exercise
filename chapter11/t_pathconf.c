/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_pathconf.c
*
* Author:           garyparrot
* Created:          2019/07/20
* Description:      pathconf() fpathconf(), query file configuration during runtime
******************************************************************************/

// there is two way to retrieve system configuration when programming
// 1. header file, unisted.h or limits.h
// 2. sysconf fileconf
//
// the value from sysconf(fileconf) won't be more strict than header file

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"


void fpathconfPrint(const char* msg, int fd, int name){
    int kep = errno; errno = 0;
    long lim = fpathconf(fd, name);

    if(lim != -1)
        printf("%s %ld\n", msg, lim);
    else{
        if(errno == 0)
            // sometime system doesn't given the strict value, fpathconf or sysconf
            // will tell you it is indeterminated.
            //
            // You can, Pick SUSv3's lowest value as the striction.
            //          Assume a legal value
            //          GNU Autoconf, generate a header file about system information and restriction 
            printf("%s (indeterminate or not supported)", msg);
        else
            errExit("fpathconf");
    }

    errno = kep;
}

int main(int argc, const char *argv[]){

    fpathconfPrint("_PC_PATH_MAX:     ", STDIN_FILENO, _PC_PATH_MAX);
    fpathconfPrint("_PC_NAME_MAX:     ", STDIN_FILENO, _PC_NAME_MAX);
    fpathconfPrint("_PC_MAX_INPUT:    ", STDIN_FILENO, _PC_MAX_INPUT);
    
    return 0;
}
