/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_sysconf.c
*
* Author:           garyparrot
* Created:          2019/07/20
* Description:      Demonstrating how to use sysconf
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

void sysconfPrint(const char *msg, int name){
    int kep = errno; errno = 0;
    // sysconf used to query system configuration during runtime.
    // its return value will be long.
    // According to SUSv3, the value returned by sysconf shouldn't change during
    // the life time of that process, but in linux we can use setrlimit() to change
    // some of the configuration during runtime.
    long lim = sysconf(name);

    if(lim != -1)
        printf("%s %ld\n", msg, lim);
    else{
        if(errno == 0)
            printf("%s (indeterminate or not supported)\n",msg);
        else
            errExit("sysconf");
    }

    errno = kep;
}

int main(int argc, const char *argv[]){

    sysconfPrint("_SC_ARG_MAX:        ", _SC_ARG_MAX);
    sysconfPrint("_SC_LOGIN_NAME_MAX: ", _SC_LOGIN_NAME_MAX);
    sysconfPrint("_SC_OPEN_MAX:       ", _SC_OPEN_MAX);
    sysconfPrint("_SC_NGROUPS_MAX:    ", _SC_NGROUPS_MAX);
    sysconfPrint("_SC_PAGESIZE:       ", _SC_PAGESIZE);
    sysconfPrint("_SC_RTSIG_MAX:      ", _SC_RTSIG_MAX);
    
    return 0;
}
