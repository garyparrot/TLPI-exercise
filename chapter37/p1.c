/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/09/13
* Description:      exercise 1
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    if(argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s level message", argv[0]);

    int level;
    switch(argv[1][0]){
        case 'E': level = LOG_EMERG;   break;
        case 'A': level = LOG_ALERT;   break;
        case 'C': level = LOG_CRIT;    break;
        case 'R': level = LOG_ERR;     break;
        case 'W': level = LOG_WARNING; break;
        case 'N': level = LOG_NOTICE;  break;
        case 'I': level = LOG_INFO;    break;
        case 'D': level = LOG_DEBUG;   break;
        default: errExit("Unknow tag");
    }

    syslog(level, argv[2]);
    
    return 0;
}
