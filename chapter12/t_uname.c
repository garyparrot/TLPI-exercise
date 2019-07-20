/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_uname.c
*
* Author:           garyparrot
* Created:          2019/07/20
* Description:      demonstrating how to use uname.
******************************************************************************/

#define _GNU_SOURCE /* utsname.domainname */

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <sys/utsname.h> /* uname */

int main(int argc, const char *argv[]){

    struct utsname utm;

    if(uname(&utm) != 0) errExit("uname");

    printf("System name : %s\n", utm.sysname);
    printf("Node name   : %s\n", utm.nodename);
    printf("Release     : %s\n", utm.release);
    printf("Version     : %s\n", utm.version);
    printf("Machine     : %s\n", utm.machine);
#ifdef _GNU_SOURCE
    printf("Domain name : %s\n", utm.domainname);
#endif
    
    return 0;
}
